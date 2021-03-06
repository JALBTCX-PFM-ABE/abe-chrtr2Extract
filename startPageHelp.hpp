
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



QString chrtr2_fileText = 
  startPage::tr ("Use the browse button to select an input CHRTR2 file.  You cannot modify the text in "
                 "the <b>CHRTR2 File</b> text window.  The reason for this is that the file must exist in order for the "
                 "program to run.  Note also that the <b>Next</b> button will not be enabled until you select an input "
                 "CHRTR2 file.  When one is selected the default output base name will be supplied in the <b>Output file "
                 "base name</b> text window.");

QString chrtr2_fileBrowseText = 
  startPage::tr ("Use this button to select the input CHRTR2 file");

QString output_fileText = 
  startPage::tr ("This is the base name for the output files.  The assumption is that the files will be placed in the "
                 "current working directory.  If you want to place them elsewhere you must specify a complete path such "
                 "as <b><i>/data1/datasets/palau/sheets/sheet02</i></b>.  If you do not enter a basename then the chrtr2 "
                 "file name will be used.");

QString area_fileText = 
  startPage::tr ("You may enter an area file name to be used to limit the area of the CHRTR2 file from which to extract "
                 "data.  The area file may be in the ISS-60 survey planner area format (.ARE), the NAVO generic area "
                 "format (.are), the Army Corps area format (.afs), or ESRI shape file format.  Shape files must be "
                 "either Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM format and must be geographic "
                 "(not projected).  For PolyLine files the first point will be duplicated to close the polygon.  "
                 "Whether using Polygon or PolyLine files, only the first shape will be used if there are multiple "
                 "shapes in the file.<br><br>"
                 "Generic area format files (.are) contain a simple list of polygon points.  The points may be in any of the "
                 "following formats:"
                 "<ul>"
                 "<li>Hemisphere Degrees Minutes Seconds.decimal</li>"
                 "<li>Hemisphere Degrees Minutes.decimal</li>"
                 "<li>Hemisphere Degrees.decimal</li>"
                 "<li>Sign Degrees Minutes Seconds.decimal</li>"
                 "<li>Sign Degrees Minutes.decimal</li>"
                 "<li>Sign Degrees.decimal</li>"
                 "</ul>"
                 "The lat and lon must be entered one per line, separated by a comma.  You do not need to repeat the "
                 "first point, the polygon will be closed automatically.");

QString area_fileBrowseText = 
  startPage::tr ("Use this button to select an optional area file.");
