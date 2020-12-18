
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




#ifndef VERSION

#define     VERSION     "PFM Software - chrtr2Extract V2.13 - 10/17/16"

#endif

/*! <pre>

    Version 1.00
    Jan C. Depner
    12/16/10

    First version.


    Version 1.01
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 1.02
    Jan C. Depner
    03/02/11

    Bug fix handling output file name.  Also, when creating a new area file from NSEW input, name it after the output
    file, not the input file.


    Version 1.03
    Jan C. Depner
    03/22/11

    Added ASCII grid output option.


    Version 1.04
    Jan C. Depner
    06/27/11

    Save all directories used by the QFileDialogs.  Add current working directory to the sidebar for all QFileDialogs.


    Version 1.05
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 1.06
    Jan C. Depner
    11/30/11

    Now outputs .dbf and .prj files along with the default .shp and .shx files so that
    Arc can read them correctly.
    Converted .xpm icons to .png icons.

    
    Version 2.0
    Stacy Johnson
    08/20/2012

    Removed CHRTR2_FATHOMS option from the dropdown list at the insistence of Paul Marin.  Change was approved by Wade Ladner.


    Version 2.01
    Stacy Johnson
    01/28/2013

    if wlon > 180, convert to 180 world.


    Version 2.02
    Stacy Johnson
    11/19/2013

    Correct row/col calculation when extracting a chrtr2


    Version 2.03
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 2.04
    Jan C. Depner (PFM Software)
    02/26/14

    Cleaned up "Set but not used" variables that show up using the 4.8.2 version of gcc.


    Version 2.05
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 2.06
    Jan C. Depner (PFM Software)
    07/05/14

    - Had to change the argument order in pj_init_plus for the UTM projection.  Newer versions of 
      proj4 appear to be very sensitive to this.


    Version 2.07
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 2.08
    Jan C. Depner (PFM Software)
    03/31/15

    - Added ability to use ESRI Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM geographic shape
      files as area files.


    Version 2.09
    Jan C. Depner (PFM Software), Jim Hammack (New Wave Systems)
    04/03/15

    - Computes zone prior to defining UTM projection so that we don't need PROJ_LIB set (I think).


    Version 2.10
    Jan C. Depner (PFM Software)
    06/27/15

    - Fixed PROJ4 init problem.


    Version 2.11
    Jan C. Depner (PFM Software)
    07/12/16

    - Removed COMPD_CS from Well-known Text.


    Version 2.12
    Jan C. Depner (PFM Software)
    08/26/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 2.13
    Jan C. Depner (PFM Software)
    10/17/16

    - Put COMPD_CS back on Well-known Text (GDAL/OGR supports it but QGIS hasn't caught up yet).

</pre>*/
