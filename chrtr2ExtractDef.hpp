
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



#ifndef __CHRTR2EXTRACTDEF_H__
#define __CHRTR2EXTRACTDEF_H__


#include <cerrno>

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <proj_api.h>

#include "shapefil.h"

#include "nvutility.h"
#include "nvutility.hpp"

#include "chrtr2.h"
#include "llz.h"


#define       CH2EX_ASCII                 0
#define       CH2EX_ASCII_GRID            1
#define       CH2EX_LLZ                   2
#define       CH2EX_RDP                   3
#define       CH2EX_SHP                   4
#define       CH2EX_CHRTR2                5


typedef struct
{
  uint8_t     z_units;                    //  Z units, either CHRTR2_METERS, CHRTR2_FEET, CHRTR2_FATHOMS, or CHRTR2_FATHOMS_AT_4800
  int8_t      format;                     //  CH2EX_ASCII, CH2EX_ASCII_GRID, CH2EX_LLZ, CH2EX_RDP, CH2EX_SHP, CH2EX_CHRTR2
  uint8_t     chk;                        //  Only include checked bins
  uint8_t     flp;                        //  Invert sign of Z values
  uint8_t     utm;                        //  Convert position to UTM
  uint8_t     unc;                        //  Include uncertainty data in ASCII output
  uint8_t     cut;                        //  Apply the depth cutoff
  double      cutoff;                     //  Depth cutoff value
  double      datum_shift;                //  Datum shift to be added
  int8_t      size;                       //  0 - no limit, 1 - 1GB, 2 - 2GB, 3 - 4GB
  QString     input_dir;
  QString     area_dir;
  int32_t     window_x;
  int32_t     window_y;
  int32_t     window_width;
  int32_t     window_height;
  QFont       font;                       //  Font used for all ABE GUI applications
} OPTIONS;


typedef struct
{
  QGroupBox           *ebox;
  QProgressBar        *ebar;
} RUN_PROGRESS;


#endif
