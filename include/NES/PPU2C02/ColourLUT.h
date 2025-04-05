#ifndef COLOUR_LUT_H
#define COLOUR_LUT_H

#include <cstdint>

/**
* Class representing
* an RGB colour.
*/
class Colour {
public:

    using Byte = uint8_t;

    /**
    * Class constructor.
    * Initializes a class
    * instance with default
    * values.
    */
    Colour(void) :
        mR(0),
        mG(0),
        mB(0)
    {}

    /**
    * Class constructor.
    * Initializes a class
    * instance with given
    * values.
    * 
    * @param red red value
    * @param green green value
    * @param blue blue value
    */
    Colour(Byte red, Byte green, Byte blue) :
        mR(red),
        mG(green),
        mB(blue)
    {}

    /**
    * Returns the red value
    * of the color.
    * 
    * @return red value of the colour
    */
    Byte red(void) const { return mR; }

    /**
    * Returns the green value
    * of the color.
    * 
    * @return green value of the colour
    */
    Byte green(void) const { return mG; }

    /**
    * Returns the blue value
    * of the color.
    * 
    * @return blue value of the colour
    */
    Byte blue(void) const { return mB; }

private:

    /** Red value of the colour */
    Byte mR;

    /** Green value of the colour */
    Byte mG;

    /** Blue value of the colour */
    Byte mB;
};

/**
* A lookup table for
* colours supported by
* by the NES PPU.
* 
* @see PPU
*/
class ColourLUT {

    using Byte = uint8_t;

    friend class PPU2C02;   //only PPU can create this LUT

    /**
    * Returns the colour for
    * the fiven colour code.
    * 
    * @param code colour code
    * 
    * @return fetched colour
    */
    const Colour& operator[](const Byte& code) const { return mColours[code & 0x3F]; }

    /**
    * An array of colours
    * supported by the 
    * NES' PPU.
    */
    Colour mColours[64] = {
        Colour(84, 84, 84),
        Colour(0, 30, 116),
        Colour(8, 16, 144),
        Colour(48, 0, 136),
        Colour(68, 0, 100),
        Colour(92, 0, 48),
        Colour(84, 4, 0),
        Colour(60, 24, 0),
        Colour(32, 42, 0),
        Colour(8, 58, 0),
        Colour(0, 64, 0),
        Colour(0, 60, 0),
        Colour(0, 50, 60),
        Colour(0, 0, 0),
        Colour(0, 0, 0),
        Colour(0, 0, 0),

        Colour(152, 150, 152),
        Colour(8, 76, 196),
        Colour(48, 50, 236),
        Colour(92, 30, 228),
        Colour(136, 20, 176),
        Colour(160, 20, 100),
        Colour(152, 34, 32),
        Colour(120, 60, 0),
        Colour(84, 90, 0),
        Colour(40, 114, 0),
        Colour(8, 124, 0),
        Colour(0, 118, 40),
        Colour(0, 102, 120),
        Colour(0, 0, 0),
        Colour(0, 0, 0),
        Colour(0, 0, 0),

        Colour(236, 238, 236),
        Colour(76, 154, 236),
        Colour(120, 124, 236),
        Colour(176, 98, 236),
        Colour(228, 84, 236),
        Colour(236, 88, 180),
        Colour(236, 106, 100),
        Colour(212, 136, 32),
        Colour(160, 170, 0),
        Colour(116, 196, 0),
        Colour(76, 208, 32),
        Colour(56, 204, 108),
        Colour(56, 180, 204),
        Colour(60, 60, 60),
        Colour(0, 0, 0),
        Colour(0, 0, 0),

        Colour(236, 238, 236),
        Colour(168, 204, 236),
        Colour(188, 188, 236),
        Colour(212, 178, 236),
        Colour(236, 174, 236),
        Colour(236, 174, 212),
        Colour(236, 180, 176),
        Colour(228, 196, 144),
        Colour(204, 210, 120),
        Colour(180, 222, 120),
        Colour(168, 226, 144),
        Colour(152, 226, 180),
        Colour(160, 214, 228),
        Colour(160, 162, 160),
        Colour(0, 0, 0),
        Colour(0, 0, 0)
    };

};

#endif // !COLOUR_LUT_H
