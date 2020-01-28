////// Instantly applyable search flags

if (isBit(flags, FLAG_NOCOLOR)) {
COLOR_GREEN_ASTERIX = COLOR_HEADER = COLOR_IMPORTANT = COLOR_LIGHT = COLOR_NORMAL = "";
}


////// Additional checks



////// Load installed

sprintf(db_main, "%s/%s", getenv("HOME"), DB);
sprintf(db_installed, "%s/%s-installed", getenv("HOME"), DB);
// TODO: Add option to disable database time check

long length;
FILE *installedFile = fopen (db_installed, "r");
if (installedFile == NULL) {
fprintf(stderr, "Error opening file");
return 1;
}

if (installedFile) {
fseek (installedFile, 0, SEEK_END);
length = ftell (installedFile);
fseek (installedFile, 0, SEEK_SET);
installedBuffer = malloc (length);
if (installedBuffer) {
fread (installedBuffer, 1, length, installedFile);
}
fclose (installedFile);
}

////// Actual search

printf("\n");
if (searchFile(&ex, installedBuffer, flags)) {
free(installedBuffer);
return -127;
}
free(installedBuffer);
regfree(&ex);

////// Check database age
if (!isBit(flags, FLAG_NOWARNDB)) {
dbAgeCheck(db_main);
}

return 0;
