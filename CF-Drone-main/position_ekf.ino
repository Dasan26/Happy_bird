// EKF for vertical position, vertical speed and accelerometer bias.
// State: [z (m), vz (m/s), accelerometer bias (m/s^2)].
// Prediction uses attitude-compensated IMU acceleration; correction uses DPS310 altitude.

static float verticalState[3] = {0.0f, 0.0f, 0.0f};
static float verticalCov[3][3] = {
	{0.25f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.25f}
};
static bool verticalEKFIsReady = false;

// Measurement/process noise; tune only from recorded stationary and flight data.
static const float BARO_ALTITUDE_VARIANCE = 0.09f; // sigma ~= 0.30 m
static const float ACCEL_PROCESS_VARIANCE = 4.0f;  // (m/s^2)^2
static const float ACCEL_BIAS_RANDOM_WALK = 0.02f;

void initializeVerticalEKF(float altitudeMeters) {
	verticalState[0] = altitudeMeters;
	verticalState[1] = 0.0f;
	verticalState[2] = 0.0f;
	memset(verticalCov, 0, sizeof(verticalCov));
	verticalCov[0][0] = BARO_ALTITUDE_VARIANCE;
	verticalCov[1][1] = 1.0f;
	verticalCov[2][2] = 0.25f;
	verticalEKFIsReady = true;
}

bool verticalEKFReady() { return verticalEKFIsReady; }
float verticalEKFAltitude() { return verticalState[0]; }
float verticalEKFSpeed() { return verticalState[1]; }

void predictVerticalEKF(float worldVerticalAcceleration, float sampleDt) {
	if (!verticalEKFIsReady || !isfinite(worldVerticalAcceleration) ||
	    !isfinite(sampleDt) || sampleDt <= 0.0f || sampleDt > 0.05f) return;

	const float dt2 = sampleDt * sampleDt;
	const float F[3][3] = {
		{1.0f, sampleDt, -0.5f * dt2},
		{0.0f, 1.0f,    -sampleDt},
		{0.0f, 0.0f,     1.0f}
	};
	float correctedAccel = worldVerticalAcceleration - verticalState[2];
	verticalState[0] += verticalState[1] * sampleDt + 0.5f * correctedAccel * dt2;
	verticalState[1] += correctedAccel * sampleDt;

	float FP[3][3] = {};
	float predictedCov[3][3] = {};
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) FP[i][j] += F[i][k] * verticalCov[k][j];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) predictedCov[i][j] += FP[i][k] * F[j][k];

	const float q = ACCEL_PROCESS_VARIANCE;
	predictedCov[0][0] += q * dt2 * dt2 * 0.25f;
	predictedCov[0][1] += q * dt2 * sampleDt * 0.5f;
	predictedCov[1][0] += q * dt2 * sampleDt * 0.5f;
	predictedCov[1][1] += q * dt2;
	predictedCov[2][2] += ACCEL_BIAS_RANDOM_WALK * sampleDt;
	memcpy(verticalCov, predictedCov, sizeof(verticalCov));
}

void correctVerticalEKF(float barometerAltitude) {
	if (!verticalEKFIsReady || !isfinite(barometerAltitude)) return;
	const float innovation = barometerAltitude - verticalState[0];
	const float innovationVariance = verticalCov[0][0] + BARO_ALTITUDE_VARIANCE;
	if (!isfinite(innovationVariance) || innovationVariance <= 1e-8f) return;

	float K[3];
	for (int i = 0; i < 3; i++) K[i] = verticalCov[i][0] / innovationVariance;
	for (int i = 0; i < 3; i++) verticalState[i] += K[i] * innovation;

	// Joseph covariance update for numerical stability: P=(I-KH)P(I-KH)' + KRK'.
	float A[3][3] = {};
	float AP[3][3] = {};
	float updatedCov[3][3] = {};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) A[i][j] = (i == j ? 1.0f : 0.0f) - (j == 0 ? K[i] : 0.0f);
	}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) AP[i][j] += A[i][k] * verticalCov[k][j];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) updatedCov[i][j] += AP[i][k] * A[j][k];
			updatedCov[i][j] += K[i] * BARO_ALTITUDE_VARIANCE * K[j];
		}
	}
	memcpy(verticalCov, updatedCov, sizeof(verticalCov));
}

// Discrete vertical LQI/LQR from the supplied equations, evaluated at 100 Hz.
// Input/output units: reference and z in meters; output acceleration in m/s^2.
static float altitudeErrorIntegral = 0.0f;
static float lastLQRTime = NAN;
static float lastVerticalAccelerationCommand = 0.0f;
static const float ALTITUDE_LQR_TS = 0.01f;

void resetVerticalLQR() {
	altitudeErrorIntegral = 0.0f;
	lastLQRTime = t;
	lastVerticalAccelerationCommand = 0.0f;
}

float updateVerticalLQR(float referenceMeters, float altitudeMeters, float verticalSpeedMps) {
	if (!isfinite(referenceMeters) || !isfinite(altitudeMeters) || !isfinite(verticalSpeedMps)) return 0.0f;
	if (!isfinite(lastLQRTime)) lastLQRTime = t;
	if (t - lastLQRTime < ALTITUDE_LQR_TS) return lastVerticalAccelerationCommand;
	if (t - lastLQRTime > 0.05f) lastLQRTime = t - ALTITUDE_LQR_TS;
	int updates = 0;
	while (t - lastLQRTime >= ALTITUDE_LQR_TS && updates < 5) {
		lastLQRTime += ALTITUDE_LQR_TS;
		// Regulate altitude error around the captured reference. This avoids an
		// absolute-altitude feedback term cutting thrust as soon as ALTHOLD engages.
		float positionError = altitudeMeters - referenceMeters;
		float integralPrevious = altitudeErrorIntegral;
		altitudeErrorIntegral = constrain(
			altitudeErrorIntegral - ALTITUDE_LQR_TS * positionError,
			-0.5150f, 0.5150f);
		float rawAcceleration = -14.085f * positionError
			- 5.805f * verticalSpeedMps
			+ 4.7625f * integralPrevious
			+ 0.047625f * referenceMeters;
		lastVerticalAccelerationCommand = constrain(rawAcceleration, -4.9050f, 4.9050f);
		updates++;
	}
	return lastVerticalAccelerationCommand;
}
