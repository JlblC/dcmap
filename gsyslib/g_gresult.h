#pragma once

#include "g_gsys.h"

namespace gsys
{
    class GRESULT
    {
        int value;
    public:
        operator bool(){return value > 0;}
		bool operator !(){return value <= 0;}
        bool operator == (GRESULT const& gr){return value == gr.value;}

        GRESULT(GRESULT const& gr){value = gr.value;} 
        GRESULT(int v){value = v;}
        GRESULT(bool b){value = b?1:-1;}

        GRESULT& operator = (GRESULT const& gr){value = gr.value;return *this;}

        GRESULT(){} 
    };

    // only for compatiblity with old code.
    inline bool GFailed(GRESULT gr){return !(bool)gr;}
    inline bool GSuccess(GRESULT gr){return (bool)gr;}

#define G_OK (::gsys::GRESULT(1))
#define G_FAIL (::gsys::GRESULT(-1))

#define GERR_NOTFOUND (::gsys::GRESULT(-2))
#define	GERR_INVALID_SYNTAX (::gsys::GRESULT(-3))
#define	GERR_INVALID_DATA (::gsys::GRESULT(-4))
#define	GERR_INVALID_PARAMS (::gsys::GRESULT(-5))
#define	GERR_UNKNOWN_COMMAND (::gsys::GRESULT(-6))
#define	GERR_INVALID_CALL (::gsys::GRESULT(-7))

#define G_USER_ERROR(x) (::gsys::GRESULT(-255-x))
#define G_USER_RESULT(x) (::gsys::GRESULT(255+x))

}