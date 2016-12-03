#pragma once

/*
This class is used to determine GL_ARB_texture_non_power_of_two extension
exist or not. m_bNonPowerOfTwo flag is set or reset based on the availability of 
GL_ARB_texture_non_power_of_two extension.
*/
class GLExtension
{
public:

    static GLExtension& GetInstance();

private:

    GLExtension(void);
    ~GLExtension(void);

public:

    bool m_bNonPowerOfTwo;
};
