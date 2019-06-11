#include <iostream>
#include <array>
#include <algorithm>

using namespace std;

constexpr auto PI = 3.14159265358979323846;
constexpr auto MAX_RPM = 8000.0f;

std::array<float, 21> torque = 
{
    // Torque	// Rpm		// Array location
    0,		// 0		0
    0,		// 500		1
    390.0f,	// 1000		2
    420.0f,	// 1500		3
    430.0f,	// 2000		4
    440.0f,	// 2500		5
    450.0f,	// 3000		6
    460.0f,	// 3500		7
    480.0f,	// 4000		8
    480.0f,	// 4500		9
    470.0f,	// 5000		10
    450.0f,	// 5500		11
    0,		// 6000		12
    0,		// 6500		13
    0,		// 7000		14
    0,		// 7500		15
    0,		// 8000		16
    0,		// 8500		17
    0,		// 9000		18
    0,		// 9500		19
    0		// 10000	20
};

std::array<float, 6> ratios = 
{
    // Ratio	// Gear number	// Array location
    2.66f,	// 1		// 0
    1.78f,	// 2		// 1
    1.3f,	// 3		// 2
    1.0f,	// 4		// 3
    0.74f,	// 5		// 4
    0.5f	// 6		// 5
};

class CarEngine
{
    private:
        // Parameters to affect engine
        int gear;
        float throttle;
        float brake;

        float velocity;
        float differentialRatio;
        float transmissionEfficiency;
        float wheelRadius;
        float mass;
        float coefficientOfFriction;
        float frontalArea;
        float airDensity;
        float brakingForce;

        float CalculateRpm()
        {
            // calculate rpm from v
            float wheelAngularVelocity = velocity / wheelRadius;
            float rpm = wheelAngularVelocity * ratios[gear - 1] * differentialRatio;

            // convert rpm
            rpm *= (60.0f / (2 * (float)M_PI));
            return rpm;
        }

        float GetTorque(float rpm)
        {
            float roundRpm = std::max(0.f, std::min(MAX_RPM, rpm));

            // Get the torque array location for the rpm
            // The closest array location is searched and the
            // correct torque is then interpolated between the
            // current array location and the next one
            float rpmIncrementInTorqueArray = 500.0f;
            float floatingArrayPosition = roundRpm / rpmIncrementInTorqueArray;
            int exactArrayPosition = (int)floatingArrayPosition;
            float interpolationAmount = floatingArrayPosition - (float)exactArrayPosition;

            // Get the torques around the current position
            float firstTorque = torque[exactArrayPosition];

            if(++exactArrayPosition > torque.size() - 1)
                exactArrayPosition = torque.size() - 1;

            float secondTorque = torque[exactArrayPosition];

            // Calculate the interpolated torque using the remainder
            // left from the division above
            float interpolatedTorque = (1.0f - interpolationAmount) * firstTorque + interpolationAmount * secondTorque;

            return interpolatedTorque;
        }

        float GetDrive(float rpm)
        {
            // Return the real torque the engine would give
            float maximumTorque = GetTorque(rpm);
            float engineTorque = maximumTorque * throttle;
            float gearTorque = engineTorque * ratios[gear - 1];
            float driveTorque = gearTorque * differentialRatio;
            return (driveTorque * transmissionEfficiency);
        }

        float GetDrag()
        {
            // Simply calculate the air drag, everyone knows the formula :)
            return (0.5f * coefficientOfFriction * frontalArea * airDensity * (velocity * velocity));
        }

    public:
        // Constructor
        CarEngine()
        {
            gear = 1;
            throttle = 0.f;
            brake = 0.f;

            // Could be changed upon different model
            velocity = 4.0f;
            differentialRatio = 3.42f;
            transmissionEfficiency = 0.7f;
            wheelRadius = 0.34f;
            mass = 2000.0f;
            coefficientOfFriction = 0.3f;
            frontalArea = 2.2f;
            airDensity = 1.29f;
            brakingForce = 1000.0f;
        }

        // Destructor
        ~CarEngine()
        {

        }

        void Update()
        {
            float acceleratingForce = GetDrive(CalculateRpm()) - GetDrag() - (brake * brakingForce);
            velocity += acceleratingForce / mass;

            if(velocity < 0)
                velocity = 0;
        }

        int GetGear()
        {
            return gear; 
        }

        void SetGear(int value)
        {
            gear = std::max(1, std::min(value, static_cast<int>(ratios.size())));
        }

        float GetThrottle()
        {
            return throttle;
        }

        void SetThrottle(float value)
        {
            throttle = std::max(0.f, std::min(value, 1.0f));
        }

        float GetBrake()
        {
            return brake;
        }

        void SetBrake(float value)
        {
            brake = std::max(0.f, std::min(value, 1.0f));
        }

        float GetVelocity()
        {
            return velocity;
        }

        float GetRpm()
        {
            return CalculateRpm();
        }
};

int main()
{
    return 0;
}
