#include "sbos.h"
#include "thing.h"
int main()
{
    Thing thing;
    thing.Initialize();
    thing.Run();
    thing.CleanUp();
    return 0;
}