class MPU5060: public InputSource {
    const int ACCEL_XOUT_HIGH = 0x3B;
    const int ACCEL_XOUT_LOW = 0x3C;
    const int ACCEL_YOUT_HIGH = 0x3D;
    const int ACCEL_YOUT_LOW = 0x3E;
    const int ACCEL_ZOUT_HIGH = 0x3F;
    const int ACCEL_ZOUT_LOW = 0x40;

    const int I2C_ADDRESS = 0x68;

    const int PWR_MGMT_1 = 0x6B;
    const int CONFIG_REG = 0x1A;
    const int ACCEL_CONFIG_REG = 0x1C;

public:
    void onStart() override {
        Wire.begin();

        // Turn on the sensor
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(PWR_MGMT_1);
        Wire.write(0);
        Wire.endTransmission(true);

        // Configure the filter
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(CONFIG_REG);
        Wire.write(3);
        Wire.endTransmission(true);

        // Configure the accel range
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(ACCEL_CONFIG_REG);
        // 4G
        Wire.write(2 << 3);
        Wire.endTransmission(true);
    }

    void onStop() override {
        Wire.beginTransmission(I2C_ADDRESS);
        // Turn off the sensor
        Wire.write(PWR_MGMT_1);
        Wire.write(1);
        Wire.endTransmission(true);
        Wire.end();
    }

    InputEvent read() override {
        EVERY_N_MILLISECONDS(5) {
            Wire.beginTransmission(I2C_ADDRESS);
            Wire.write(ACCEL_XOUT_HIGH);
            Wire.endTransmission(false);
            Wire.requestFrom(I2C_ADDRESS, 6);
            const int16_t accelX = Wire.read() << 8 | Wire.read();
            const int16_t accelY = Wire.read() << 8 | Wire.read();
            const int16_t accelZ = Wire.read() << 8 | Wire.read();
            const uint16_t accelSum = abs(accelX) + abs(accelY) + abs(accelZ);
            const uint16_t delta = abs(m_value.value() - accelSum);
            m_value.add(accelSum);
            if (delta > 32) {
                return InputEvent{InputEvent::Acceleration, delta};
            }
        }
        return InputEvent{};
    }

    template<typename T, uint8_t Size = 8>
    struct Averager {
        std::array<T, Size> buf;
        unsigned int idx = 0;
        unsigned int count = 0;

        void add(const T &value) {
            buf[idx] = value;
            idx = (idx + 1) % Size;
            if (count < Size) {
                count += 1;
            }
        }

        T value() const {
            if (count == 0) {
                return T{};
            }
            long long int sum = 0;
            for(unsigned int i = 0; i < count; i++) {
                sum += buf[i];
            }
            return sum / count;
        }
    };

    Averager<int16_t, 32> m_value;
};
