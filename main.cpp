#include "App.h"

int main()
{
    App app;
    if (!app.Init())
        return -1;

    app.Run();
    app.Cleanup();
    return 0;
}
