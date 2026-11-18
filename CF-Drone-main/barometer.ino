// DPS310 barometric altitude and vertical speed estimate over I2C.
// Install "Adafruit DPS310" from Arduino Library Manager.
#include <Wire.h>
#include <Adafruit_DPS310.h>
#include "board_config.h"

#if BOARD_I2C_SDA >= 0 && BOARD_I2C_SCL >= 0
static Adafruit_DPS310 dps;
static bool dpsReady = false;
static float baroAltitude = NAN;       // relative altitude, meters
static float baroAltitudeReference = NAN;
static float baroLastMeasurementTime = NAN;

extern bool verticalEKFReady();
extern void initializeVerticalEKF(float altitudeMeters);
extern void correctVerticalEKF(float barometerAltitude);
extern float verticalEKFAltitude();
extern float verticalEKFSpeed();

void setupBarometer() {
	Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
	if (!dps.begin_I2C(0x77, &Wire) && !dps.begin_I2C(0x76, &Wire)) {
		print("No se detectó el DPS310 en 0x77/0x76; modo Altura deshabilitado\n");
		return;
	}
	dps.configurePressure(DPS310_64HZ, DPS310_8SAMPLES);
	dps.configureTemperature(DPS310_64HZ, DPS310_8SAMPLES);
	dpsReady = true;
	print("DPS310 listo por I2C\n");
}

void updateBarometer() {
	if (!dpsReady || !dps.pressureAvailable()) return;
	float rawAltitude = dps.readAltitude(1013.25f);
	if (!isfinite(rawAltitude)) return;
	if (!isfinite(baroAltitudeReference)) {
		baroAltitudeReference = rawAltitude;
		baroAltitude = 0.0f;
		baroLastMeasurementTime = t;
		initializeVerticalEKF(0.0f);
		return;
	}
	baroAltitude = rawAltitude - baroAltitudeReference;
	correctVerticalEKF(baroAltitude);
	baroLastMeasurementTime = t;
}

bool barometerOK() {
	return dpsReady && verticalEKFReady() && isfinite(baroLastMeasurementTime) &&
	       (t - baroLastMeasurementTime) < 0.25f;
}
float getBaroAltitude() { return verticalEKFAltitude(); }
float getBaroVerticalSpeed() { return verticalEKFSpeed(); }
#else
void setupBarometer() { print("I2C no configurado en esta placa; DPS310/modo Altura deshabilitado\n"); }
void updateBarometer() {}
bool barometerOK() { return false; }
float getBaroAltitude() { return NAN; }
float getBaroVerticalSpeed() { return NAN; }
#endif
