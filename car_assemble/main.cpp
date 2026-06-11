#include "Util.h"

#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include "CarAssembler.h"

int main()
{
    CarAssembler assembler;
    assembler.run();
    return 0;
}

#endif
