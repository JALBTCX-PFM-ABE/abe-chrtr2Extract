
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



QString unitsText = 
  optionsPage::tr ("Select the units you want to use for the output data.  If you select <b>Fathoms @ 4800 ft/sec</b> as your output units "
                   "fathoms will be converted from nominal depth using a sound velocity of 1500 meters per second to nominal dept using a "
                   "sound velocity of 4800 feet per second.  Unless you really know what you're doing you should not use <b>Fathoms @ 4800 "
                   "ft/sec</b>.");

QString formatText = 
  optionsPage::tr ("Select the output format.  Either ASCII, ASCII grid, LLZ, RDP, SHP, or CHRTR2.  ASCII data will be output as LAT, LON, Z "
                   "in decimal degrees unless the UTM option is selected.  In that case it will be ZONE, EASTING, NORTHING, Z.  ASCII grid "
                   "outputs a header consisting of wlon, elon, slat, nlat, lon_grid_size_degrees, lat_grid_size_degrees, width, height, "
                   "min_z, and max_z space delimited.  Following the header will be <b>height</b> records (south to north) of <b>width</b> "
                   "length containing space delimited values (west to east). The SHP format is ESRI's POINTZ SHAPEFILE format.  CHRTR2 can "
                   "only be selected if you selected an optional area file on the start page (it makes no sense otherwise).  If you select "
                   "CHRTR2 there will be no conversions done.  This is just for creating a CHRTR2 subset of the original input CHRTR2.");

QString checkedText = 
  optionsPage::tr ("Check this box if you only want to output data from bins that have been marked as checked.  This option is ignored "
                   "for CHRTR2 output format.");

QString flipText = 
  optionsPage::tr ("Check this box if you want to invert the sign of the Z values.  That is, from depth to elevation.  This option is "
                   "ignored for CHRTR2 output format.");

QString utmText = 
  optionsPage::tr ("Check this box if you want to output position data in UTM coordinates.  This is only meaningful if you have "
                   "selected ASCII output format.");

QString uncertText = 
  optionsPage::tr ("Check this box if you want to total uncertainty data along with the Y, X, and Z data.  This is only meaningful "
                   "if you have selected ASCII output format.");

QString cutText = 
  optionsPage::tr ("Set this flag to apply the depth cutoff value.  This option is ignored for CHRTR2 output format.");

QString cutoffText = 
  optionsPage::tr ("Set a depth cutoff value.  Any points below this value will be output as this value.  This can be used to "
                   "set a flat water surface for building DEMs.  This value is checked <b>prior</b> to the datum shift being "
                   "applied and <b>prior</b> to sign inversion.  The units are whatever you set for output so make sure you've got it "
                   "right if you're outputting feet or fathoms.  It will also be affected by the <b>Fathoms at 4800 feet per second</b> "
                   "flag.  This option is ignored for CHRTR2 output format.");

QString datumShiftText = 
  optionsPage::tr ("Set a datum shift value.  This value will be <b>added</b> to the data value <b>prior</b> to sign inversion.  "
                   "This can be used to shift from MSL to MLLW or MLLW to ellipsoid heights.  The units are whatever you set for "
                   "output so make sure you've got it right if you're outputting feet or fathoms.  It will also be affected by the "
                   "<b>Fathoms at 4800 feet per second</b> flag.  This option is ignored for CHRTR2 output format.");

QString sizeText = 
  optionsPage::tr ("Select the file size limit for the output files.  If this is set to anything other than <b>No limit</b>, files "
                   "will be broken up into multiple files with the selected max file size.  The 2GB and 4GB file sizes are approximate "
                   "due to system overhead (we subtract 65536 from the size to cover hidden file system use).  This option is ignored "
                   "for CHRTR2 output format.");
