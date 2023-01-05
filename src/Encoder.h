#include <Arduino.h>

class Encoder
{
private:
    int encA_P;
    int encC_P;
    int encB_P;
    int encSw_P;
    bool swValue;
    bool swValueOld;
    bool encA = false;
    bool encB = false;
    bool encAOld = false;
    bool encBOld = false;
    volatile int enc = 0;

public:
    Encoder(int _encA_P, int _encB_P, int _encC_P, int _encSw_P)
    {
        encA_P = _encA_P;
        encB_P = _encB_P;
        encC_P = _encC_P;
        encSw_P = _encSw_P;

        On();
    }
    void IrqEncoder()
    {
        encA = digitalRead(encA_P);
        encB = digitalRead(encB_P);

        if ((encAOld && encA && !encBOld && encB)
            //    ||(encAOld && !encA && encB && encBOld)
            //    ||(!encAOld && !encA && encBOld && !encB)
            //    ||(!encAOld && encA && !encBOld && !encB)
        )
        {
            enc = 1;
        }
        if ((encAOld && encA && encBOld && !encB)
            //    ||(!encAOld && encA && encB && encBOld)
            //    ||(!encAOld && !encA && !encBOld && encB)
            //   ||(encAOld && !encA && !encBOld && !encB)
        )
        {
            enc = -1;
        }
        encAOld = encA;
        encBOld = encB;
    }
    int ReadEnc()
    {
        int data = enc;
        enc = 0;
        return data;
    }
    int ReadBtn()
    {
        swValue = !digitalRead(encSw_P);
        if (swValue == false && swValueOld == false)
            return 0;
        if (swValue == true && swValueOld == true)
            return 2;
        if (swValue == true && swValueOld == false)
        {
            swValueOld = swValue;
            delay(10);
            return 1;
        }
        swValueOld = swValue;
        delay(10);
        return 3;
    }
    void On()
    {
        encA = digitalRead(encA_P);
        encB = digitalRead(encB_P);
        encAOld = encA;
        encBOld = encB;
        digitalWrite(encC_P, false);
    }

    void Off()
    {
        digitalWrite(encC_P, true);
    }
};