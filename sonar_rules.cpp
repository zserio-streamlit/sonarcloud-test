#include <stdint.h>
#include "sonar_rules.h"

// Signed and unsigned types should not be mixed in expressions
static void s845()
{
    // official example
    {
        int32_t a = 10;
        uint32_t b = a; // noncompliant
        int32_t c = (a > 0) ? a : b; // noncompliant
        if (a > b) // noncompliant
        {
            a = c;
        }
    }

    // 16-bit
    {
        int16_t a = 10;
        uint16_t b = a; // noncompliant
        int16_t c = (a > 0) ? a : b; // noncompliant
        if (a > b) // noncompliant - doesn't work probably due to integral promotions?
        {
            a = c;
        }
    }

    // mix signed 32 and unsigned 16
    {
        int32_t a = 10;
        uint16_t b = 11;
        int32_t c  = (a > b) ? a : b; // noncompliant - doesn't work probably due to integral promotions?

        (void)c;
    }

    // shift unsigned 16 using unsigned 8
    {
        uint16_t a = 0x00FF;
        uint8_t b = 8;
        // a << b will become 'int' due to integral promotions, why is it ok to assign int to uint16_t without cast?
        uint16_t c = a << b;

        (void)c;
    }

    // multiple binary or-s
    {
        uint16_t a = 0x0F;
        uint16_t b = 0xF0;
        uint16_t c = 0xFF00;
        // a | b will become 'int' due to integral promotions
        // - why is it ok to do or with c without cast
        // a | b | c will become 'int' due to integral promotions
        // - why is it ok to assign to int to uint16_t without cast?
        uint16_t d = a | b | c;
        uint16_t e = static_cast<uint16_t>(a | b) | c;

        (void)e;
    }
}

// Results of ~ and << operations on operands of underlying types unsigned char and unsigned short should immediately be cast to the operand's underlying type
static void s853()
{
    // original example
    {
        uint8_t port = 0x5aU;
        uint8_t result_8;
        uint16_t result_16;
        uint16_t mode;
        result_8 = (~port) >> 4; // noncompliant
        result_16 = ((port << 4) & mode) >> 6; // noncompliant
    }
}

static auto s874_f(int i)
{
    return 1 << i; // noncompliant
}

// Bitwise operators should not be applied to signed operands
static void s874()
{
    // original example
    {
        uint16_t a = 0xff;
        int16_t b = 0x0f;
        if (( a & b ) == 0x1234U) // noncompliant until C++20
        {}

        if (~b == 0x1234U) // noncompliant until C++20
        {
            s874_f(b);
        }
    }
}

void sonar_rules()
{
    s845();
    s853();
    s874();
}