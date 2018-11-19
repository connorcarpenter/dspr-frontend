#pragma once

//
// Created by connor on 11/18/18.
//

#include <Sova/References/Refable.h>
#include <Sova/Common/String.h>

namespace DsprFrontend {
    class StringScanner : public Sova::Refable {
    public:
        StringScanner(Ref<Sova::String> str);
        bool EqualsUntil(const char *str, char separator);

        void Advance(unsigned int i);

        char* CstrAndAdvanceUntil(char separator);

        void CleanCstr();

        bool IsAtEnd();

    private:
        Ref<Sova::String> theString = Null<Sova::String>();
        unsigned int index = 0;
        char* theCstr = nullptr;


    };
}
