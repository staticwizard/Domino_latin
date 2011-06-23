//testing the exporter functions out

#include <string.h>
#include "exporter.h"

int main(int argc, char* argv[]) 
{
    ExportInfo info;
    strncpy(info.tagEvent,"This is an event", 100);
    strncpy(info.tagSite, "This is a site",100);
    strncpy(info.tagDate, "This is a date", 25);
    strncpy(info.tagRound,"1", 3);
    strncpy(info.tagTeam1, "This is a team",100);
    strncpy(info.tagTeam2, "This is a team 2", 100);
    strncpy(info.tagResult,"This is a result", 10);

    //try the export function
    exportToFile ("./exportTest.txt", &info);

    //done testing;
}
