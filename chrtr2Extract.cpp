
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



#include "chrtr2Extract.hpp"
#include "chrtr2ExtractHelp.hpp"



chrtr2Extract::chrtr2Extract (int32_t *argc, char **argv, QWidget *parent)
  : QWizard (parent, 0)
{
  void set_defaults (OPTIONS *options);
  void envin (OPTIONS *options);


  QResource::registerResource ("/icons.rcc");


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/chrtr2ExtractWatermark.png"));


  set_defaults (&options);


  envin (&options);


  // Set the application font

  QApplication::setFont (options.font);


  setWizardStyle (QWizard::ClassicStyle);


  setOption (HaveHelpButton, true);
  setOption (ExtendedWatermarkPixmap, false);

  connect (this, SIGNAL (helpRequested ()), this, SLOT (slotHelpClicked ()));


  //  Set the window size and location from the defaults

  this->resize (options.window_width, options.window_height);
  this->move (options.window_x, options.window_y);


  setPage (0, new startPage (argc, argv, this, &options));
  setPage (1, new optionsPage (this, &options));
  setPage (2, new runPage (this, &progress, &extractList));


  setButtonText (QWizard::CustomButton1, tr("&Run"));
  setOption (QWizard::HaveCustomButton1, true);
  button (QWizard::CustomButton1)->setToolTip (tr ("Start the extraction process"));
  button (QWizard::CustomButton1)->setWhatsThis (runText);
  connect (this, SIGNAL (customButtonClicked (int)), this, SLOT (slotCustomButtonClicked (int)));


  setStartId (0);
}



void chrtr2Extract::initializePage (int id)
{
  void envout (OPTIONS *options);


  button (QWizard::HelpButton)->setIcon (QIcon (":/icons/contextHelp.png"));
  button (QWizard::CustomButton1)->setEnabled (false);

  switch (id)
    {
    case 0:
      break;

    case 1:
      break;

    case 2:
      button (QWizard::CustomButton1)->setEnabled (true);


      options.format = field ("format").toInt ();
      options.chk = field ("checked").toBool ();
      options.flp = field ("flip").toBool ();
      options.utm = field ("utm").toBool ();
      options.unc = field ("uncert").toBool ();
      options.cutoff = field ("cutoff").toDouble ();
      options.datum_shift = field ("datumShift").toDouble ();
      options.size = field ("size").toInt ();


      //  Use frame geometry to get the absolute x and y.

      QRect tmp = this->frameGeometry ();
      options.window_x = tmp.x ();
      options.window_y = tmp.y ();


      //  Use geometry to get the width and height.

      tmp = this->geometry ();
      options.window_width = tmp.width ();
      options.window_height = tmp.height ();


      envout (&options);



      break;
    }
}



void chrtr2Extract::cleanupPage (int id)
{
  switch (id)
    {
    case 0:
      break;

    case 1:
      break;

    case 2:
      break;
    }
}



void 
chrtr2Extract::slotHelpClicked ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void 
chrtr2Extract::slotCustomButtonClicked (int id __attribute__ ((unused)))
{
  FILE                *fp = NULL;
  int32_t             endian = 0x00010203, rdp_data[3], chrtr2_handle, out_handle = -1, out_count = 0, file_count = 0, polygon_count = 0, x_start, y_start,
                      width, height, llz_hnd = -1, zone = 0, type, numShapes;
  uint32_t            max_file_size = 0, size = 0;
  CHRTR2_HEADER       chrtr2_header, out_header;
  CHRTR2_RECORD       chrtr2_record;
  char                in_file[512], out_file[512], orig[512], areafile[512], prj_file[512];
  NV_I32_COORD2       coord;
  double              polygon_x[200], polygon_y[200], central_meridian, x, y, z, minBounds[4], maxBounds[4];
  NV_F64_MBR          mbr;
  LLZ_REC             llz_rec;
  LLZ_HEADER          llz_header;
  QString             string;
  SHPHandle           shp_hnd = NULL, shpHandle;
  SHPObject           *shape;
  DBFHandle           dbf_hnd = NULL;  
  FILE                *prj_fp;


  QApplication::setOverrideCursor (Qt::WaitCursor);


  button (QWizard::FinishButton)->setEnabled (false);
  button (QWizard::BackButton)->setEnabled (false);
  button (QWizard::CustomButton1)->setEnabled (false);


  QString chrtr2_file_name = field ("chrtr2_file_edit").toString ();
  QString output_file_name = field ("output_file_edit").toString ();
  QString area_file_name = field ("area_file_edit").toString ();

  switch (options.size)
    {
    case 0:
      max_file_size = 0;
      break;

    case 1:
      max_file_size = 1073676288;
      break;

    case 2:
      max_file_size = (uint32_t) (pow (2.0, 31.0) - 65536.0);
      break;

    case 3:
      max_file_size = (uint32_t) (pow (2.0, 32.0) - 65536.0);
      break;
    }


  //  I don't need to test for success here since we already opened and closed the file in startPage.cpp

  strcpy (in_file, chrtr2_file_name.toLatin1 ());
  chrtr2_handle = chrtr2_open_file (in_file, &chrtr2_header, CHRTR2_READONLY);


  mbr = chrtr2_header.mbr;
  strcpy (areafile, area_file_name.toLatin1 ());

  if (!area_file_name.isEmpty())
    {
      if (area_file_name.endsWith (".shp", Qt::CaseInsensitive))
        {
          //  Open shape file

          shpHandle = SHPOpen (areafile, "rb");


          //  Get shape file header info

          SHPGetInfo (shpHandle, &numShapes, &type, minBounds, maxBounds);


          //  Read only the first shape.

          shape = SHPReadObject (shpHandle, 0);


          mbr.wlon = mbr.slat = 999999.0;
          mbr.elon = mbr.nlat = -999999.0;


          //  Get all vertices

          for (int32_t j = 0 ; j < shape->nVertices ; j++)
            {
              polygon_x[j] = shape->padfX[j];
              polygon_y[j] = shape->padfY[j];

              mbr.wlon = qMin (polygon_x[j], mbr.wlon);
              mbr.elon = qMax (polygon_x[j], mbr.elon);
              mbr.slat = qMin (polygon_y[j], mbr.slat);
              mbr.nlat = qMax (polygon_y[j], mbr.nlat);
            }


          polygon_count = shape->nVertices;


          //  If this is a PolyLine file instead of a Polygon file we need to dupe the first point and increase the count by one.

          if (type == SHPT_ARC || type == SHPT_ARCZ || type == SHPT_ARCM)
            {
              polygon_x[shape->nVertices] = shape->padfX[0];
              polygon_y[shape->nVertices] = shape->padfY[0];
              polygon_count++;
            }


          SHPDestroyObject (shape);
          SHPClose (shpHandle);
        }
      else
        {
          get_area_ll_mbr (areafile, &polygon_count, polygon_x, polygon_y, &mbr);
        }
    }


  extractList->clear ();


  //  If we're doing UTM output, set the projection and get the zone.

  if (options.utm)
    {
      central_meridian = chrtr2_header.mbr.wlon + (chrtr2_header.mbr.elon - chrtr2_header.mbr.wlon) / 2.0;

      zone = (int32_t) (31.0 + central_meridian / 6.0);
      if (zone >= 61) zone = 60;	
      if (zone <= 0) zone = 1;

      char string[128];
      if (chrtr2_header.mbr.nlat < 0.0)
        {
          sprintf (string, "+proj=utm +zone=%d +lon_0=%.9f +ellps=WGS84 +datum=WGS84 +south", zone, central_meridian);
        }
      else
        {
          sprintf (string, "+proj=utm +zone=%d +lon_0=%.9f +ellps=WGS84 +datum=WGS84", zone, central_meridian);
        }

      if (!(pj_utm = pj_init_plus (string)))
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Error initializing UTM projection\n"));
          exit (-1);
        }

      if (!(pj_latlon = pj_init_plus ("+proj=latlon +ellps=WGS84 +datum=WGS84")))
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Error initializing latlon projection\n"));
          exit (-1);
        }
    }


  //  If we're doing this by area there is no need to go through the entire file so we'll generate starts and ends based on
  //  the mbr.

  x_start = 0;
  y_start = 0;
  width = chrtr2_header.width;
  height = chrtr2_header.height;

  if (polygon_count)
    {
      if (mbr.slat > chrtr2_header.mbr.nlat || mbr.nlat < chrtr2_header.mbr.slat ||
          mbr.wlon > chrtr2_header.mbr.elon || mbr.elon < chrtr2_header.mbr.wlon)
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Specified area is completely outside of the CHRTR2 bounds!"));
          exit (-1);
        }


      //  Match to nearest cell.

      x_start = NINT ((mbr.wlon - chrtr2_header.mbr.wlon) / chrtr2_header.lon_grid_size_degrees);
      y_start = NINT ((mbr.slat - chrtr2_header.mbr.slat) / chrtr2_header.lat_grid_size_degrees);
      width = NINT ((mbr.elon - mbr.wlon) / chrtr2_header.lon_grid_size_degrees);
      height = NINT ((mbr.nlat - mbr.slat) / chrtr2_header.lat_grid_size_degrees);


      //  Adjust to CHRTR2 bounds if necessary.

      if (x_start < 0) x_start = 0;
      if (y_start < 0) y_start = 0;
      if (x_start + width > chrtr2_header.width) width = chrtr2_header.width - x_start;
      if (y_start + height > chrtr2_header.height) height = chrtr2_header.height - y_start;


      //  Redefine bounds.

      mbr.wlon = chrtr2_header.mbr.wlon + x_start * chrtr2_header.lon_grid_size_degrees;
      mbr.slat = chrtr2_header.mbr.slat + y_start * chrtr2_header.lat_grid_size_degrees;
      mbr.elon = mbr.wlon + width * chrtr2_header.lon_grid_size_degrees;
      mbr.nlat = mbr.slat + height * chrtr2_header.lat_grid_size_degrees;
    }

  float min_z = 9999999999.0;
  float max_z = -9999999999.0;


  if (output_file_name.endsWith (".txt") || output_file_name.endsWith (".grd") || output_file_name.endsWith (".llz") || output_file_name.endsWith (".rdp") ||
      output_file_name.endsWith (".shp") || output_file_name.endsWith (".ch2")) output_file_name.chop (4);

  strcpy (orig, output_file_name.toLatin1 ());

  switch (options.format)
    {
    case CH2EX_ASCII:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.txt", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.txt", orig);
        }

      if ((fp = fopen64 (out_file, "w")) == NULL)
	{
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));
	  exit (-1);
	}
      break;

    case CH2EX_ASCII_GRID:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.grd", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.grd", orig);
        }

      if ((fp = fopen64 (out_file, "w")) == NULL)
	{
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));
	  exit (-1);
	}

      fprintf (fp, "%14.9f %14.9f %14.9f %14.9f %14.9f %14.9f %05d %05d %15.2f %15.2f\n", mbr.wlon, mbr.elon, mbr.slat, mbr.nlat,
               chrtr2_header.lon_grid_size_degrees, chrtr2_header.lat_grid_size_degrees, width, height, min_z, max_z);

      break;

    case CH2EX_LLZ:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.llz", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.llz", orig);
        }


      //  Boilerplate LLZ header.

      memset (&llz_header, 0, sizeof (LLZ_HEADER));
      sprintf (llz_header.comments, "Created from %s using %s", gen_basename (in_file), VERSION);
      llz_header.time_flag = NVFalse;
      llz_header.depth_units = 0;

      if ((llz_hnd = create_llz (out_file, llz_header)) < 0)
	{
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));
	  exit (-1);
	}
      break;

    case CH2EX_RDP:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.rdp", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.rdp", orig);
        }

      if ((fp = fopen64 (out_file, "w")) == NULL)
	{
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));
	  exit (-1);
	}
      break;

    case CH2EX_SHP:
      if (options.size)
        {
          sprintf (out_file, "%s.%02d.shp", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.shp", orig);
        }

      if ((shp_hnd = SHPCreate (out_file, SHPT_POINTZ)) == NULL)
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));
          exit (-1);
        }


      //  Making dummy DBF file so Arc won't barf.

      if ((dbf_hnd = DBFCreate (out_file)) == NULL)
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));

          exit (-1);
        }


      //  Adding a dummy field.

      if (DBFAddField (dbf_hnd, "nada", FTLogical, 1, 0) == -1)
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Error adding field to DBF file."));

          exit (-1);
        }


      //  Stupid freaking .prj file

      strcpy (prj_file, QString (out_file).replace (".shp", ".prj").toLatin1 ());
      if ((prj_fp = fopen (prj_file, "w")) == NULL)
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to create ESRI PRJ file ") +
                                 QDir::toNativeSeparators (QString (prj_file)) + 
                                 tr ("  The error message returned was:\n\n") +
                                 QString (strerror (errno)));

          exit (-1);
        }

      fprintf (prj_fp, "COMPD_CS[\"WGS84 with WGS84E Z\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],AUTHORITY[\"EPSG\",\"4326\"]],VERT_CS[\"ellipsoid Z in meters\",VERT_DATUM[\"Ellipsoid\",2002],UNIT[\"metre\",1],AXIS[\"Z\",UP]]]\n");
      fclose (prj_fp);

      break;

    case CH2EX_CHRTR2:
      sprintf (out_file, "%s.ch2", orig);


      //  Populate the chrtr2 header prior to creating the file.

      out_header = chrtr2_header;


      //  Set the Z units.

      out_header.z_units = options.z_units;


      //  Set the version.

      strcpy (out_header.creation_software, VERSION);


      //  Set the extents.

      if(mbr.wlon>180)
	{
	  mbr.wlon=mbr.wlon-360;
	  mbr.elon=mbr.elon-360;  
	}
      out_header.mbr.wlon = mbr.wlon;
      out_header.mbr.slat = mbr.slat;
      out_header.width = (int32_t) floor(((mbr.elon - mbr.wlon) / chrtr2_header.lon_grid_size_degrees)+.5)+1;
      out_header.height = (int32_t) floor(((mbr.nlat - mbr.slat) / chrtr2_header.lat_grid_size_degrees)+.5)+1;


      /*  Try to create and open the chrtr2 file.  */

      out_handle = chrtr2_create_file (out_file, &out_header);

      if (out_handle < 0)
        {
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (chrtr2_strerror ()));
          exit (-1);
        }
      break;
    }


  extractList->addItem (tr ("FILE : ") + QString (out_file));


  //  Loop over height & width of bins or area

  progress.ebar->setRange (0, height);


  //  Note: I had to static this loop variable because the optimizer was causing all kinds of havoc.  I found this by printing
  //  the value of "row" inside the loop when generating an ASCII grid.  With the print statement everything worked fine (as 
  //  long as I printed out "row").  Without the print statement the value of "value" was being set to 0.0.  I'm assuming that
  //  the optimizer was putting "row" in a register or something nasty like that.  Making this static doesn't hurt anything
  //  AFAICT.
 
  for (static int32_t row = y_start ; row < y_start + height ; row++)
    {
      coord.y = row;

      for (int32_t col = x_start ; col < x_start + width ; col++)
        {
          coord.x = col;

          chrtr2_read_record (chrtr2_handle, coord, &chrtr2_record);

          float value = chrtr2_record.z;

          if (!options.chk || (chrtr2_record.status & CHRTR2_CHECKED))
            {
              if (chrtr2_record.status != CHRTR2_NULL)
                {

                  //  First, let's convert our input data to meters.

                  switch (chrtr2_header.z_units)
                    {
                    case CHRTR2_METERS:
                      value = chrtr2_record.z;
                      break;

                    case CHRTR2_FEET:
                      value = chrtr2_record.z * 0.34080;
                      break;

                    case CHRTR2_FATHOMS_AT_4800:
                      value = chrtr2_record.z * 1.87501;
                      break;
                    }


                  //  Now convert our cutoff and/or datum shift values to meters.

                  float cutoff = options.cut;
                  float shift = options.datum_shift;

                  switch (options.z_units)
                    {
                    case CHRTR2_FEET:
                      cutoff /= 3.28084;
                      shift /= 3.28084;
                      break;

                    case CHRTR2_FATHOMS_AT_4800:
                      cutoff /= 0.53333;
                      shift /= 0.53333;
                      break;
                    }


                  if (options.cut && value > cutoff) value = cutoff;
                  value += shift;

                  if (options.flp) value = -value;


                  //  Convert our modified input value to the output units requested.

                  switch (options.z_units)
                    {
                    case CHRTR2_FEET:
                      value *= 3.28084;
                      break;

                    case CHRTR2_FATHOMS_AT_4800:
                      value *= 0.53333;
                      break;
                    }

                  double lat, lon;
                  chrtr2_get_lat_lon (chrtr2_handle, &lat, &lon, coord);


                  switch (options.format)
                    {
                    case CH2EX_CHRTR2:
                      NV_I32_COORD2 out_coord;
                      out_coord.y = row - y_start;
                      out_coord.x = col - x_start;

                      chrtr2_record.z = value;

                      chrtr2_write_record (out_handle, out_coord, chrtr2_record);

                      if (chrtr2_record.status != CHRTR2_NULL)
                        {
                          min_z = qMin (chrtr2_record.z, min_z);
                          max_z = qMax (chrtr2_record.z, max_z);
                        }
                      break;

                    case CH2EX_ASCII:
                      if (options.utm)
                        {
                          x = lon * NV_DEG_TO_RAD;
                          y = lat * NV_DEG_TO_RAD;
                          pj_transform (pj_latlon, pj_utm, 1, 1, &x, &y, NULL);

                          if (options.unc)
                            {
                              fprintf (fp, "%02d,%.2f,%.2f,%.2f,%.2f\n", zone, x, y, value, chrtr2_record.uncertainty);
                            }
                          else
                            {
                              fprintf (fp, "%02d,%.2f,%.2f,%.2f\n", zone, x, y, value);
                            }
                        }
                      else
                        {
                          if (options.unc)
                            {
                              fprintf (fp, "%.9f,%.9f,%.2f,%.2f\n", lat, lon, value, chrtr2_record.uncertainty);
                            }
                          else
                            {
                              fprintf (fp, "%.9f,%.9f,%.2f\n", lat, lon, value);
                            }
                        }
                      break;

                    case CH2EX_ASCII_GRID:
                      min_z = qMin (value, min_z);
                      max_z = qMax (value, max_z);
                      fprintf (fp, "%08.2f ", value);
                      break;

                    case CH2EX_LLZ:
                      llz_rec.xy.lat = lat;
                      llz_rec.xy.lon = lon;
                      llz_rec.depth = value;
                      llz_rec.status = 0;

                      append_llz (llz_hnd, llz_rec);
                      break;

                    case CH2EX_RDP:
                      rdp_data[0] = NINT (lat * 10000000.0);
                      rdp_data[1] = NINT (lon * 10000000.0);
                      rdp_data[2] = NINT (value * 10000.0);

                      fwrite (rdp_data, sizeof (rdp_data), 1, fp);
                      break;

                    case CH2EX_SHP:
                      z = (double) value;
                      shape = SHPCreateObject (SHPT_POINTZ, -1, 0, NULL, NULL, 1, &lon, &lat, &z, NULL);

                      SHPWriteObject (shp_hnd, -1, shape);
                      SHPDestroyObject (shape);
                      DBFWriteLogicalAttribute (dbf_hnd, 1, 0, '0');
                      break;
                    }

                  out_count++;


                  //  Make sure file size does not exceed our maximum requested output size limt.

                  if (options.size)
                    {
                      size = QFileInfo (out_file).size ();

                      if (size >= max_file_size)
                        {
                          file_count++;


                          switch (options.format)
                            {
                            case CH2EX_ASCII:
                              fclose (fp);
                              sprintf (out_file, "%s.%02d.txt", orig, file_count);

                              if ((fp = fopen64 (out_file, "w")) == NULL)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                                         tr ("\nReason:") + QString (strerror (errno)));
                                  exit (-1);
                                }
                              break;

                            case CH2EX_ASCII_GRID:

                              //  For ASCII grid output we have to add a carriage return to the last line of the previous file.

                              if (options.format == 1) fprintf (fp, "\n");


                              fclose (fp);
                              sprintf (out_file, "%s.%02d.grd", orig, file_count);

                              if ((fp = fopen64 (out_file, "w")) == NULL)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                                         tr ("\nReason:") + QString (strerror (errno)));
                                  exit (-1);
                                }
                              break;

                            case CH2EX_LLZ:
                              close_llz (llz_hnd);
                              sprintf (out_file, "%s.%02d.llz", orig, file_count);


                              //  Boilerplate LLZ header.

                              sprintf (llz_header.comments, "Created from %s using %s", gen_basename (in_file), VERSION);
                              llz_header.time_flag = NVFalse;
                              llz_header.depth_units = 0;

                              if ((llz_hnd = create_llz (out_file, llz_header)) < 0)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                                         tr ("\nReason:") + QString (strerror (errno)));
                                  exit (-1);
                                }  
                              break;

                            case CH2EX_RDP:
                              fclose (fp);
                              sprintf (out_file, "%s.%02d.rdp", orig, file_count);

                              if ((fp = fopen64 (out_file, "w")) == NULL)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                                         tr ("\nReason:") + QString (strerror (errno)));
                                  exit (-1);
                                }

                              fwrite (&endian, sizeof (int32_t), 1, fp);
                              break;

                            case CH2EX_SHP:
                              SHPClose (shp_hnd);
                              DBFClose (dbf_hnd);  

                              sprintf (out_file, "%s.%02d.shp", orig, file_count);

                              if ((shp_hnd = SHPCreate (out_file, SHPT_POINTZ)) == NULL)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") +
                                                         QString (out_file) + tr ("\nReason:") + QString (strerror (errno)));
                                  exit (-1);
                                }


                              //  Making dummy DBF file so Arc won't barf.

                              if ((dbf_hnd = DBFCreate (out_file)) == NULL)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") +
                                                         QString (out_file) + tr ("\nReason:") + QString (strerror (errno)));

                                  exit (-1);
                                }


                              //  Adding a dummy field.

                              if (DBFAddField (dbf_hnd, "nada", FTLogical, 1, 0) == -1)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Error adding field to DBF file."));

                                  exit (-1);
                                }


                              //  Stupid freaking .prj file

                              strcpy (prj_file, QString (out_file).replace (".shp", ".prj").toLatin1 ());
                              if ((prj_fp = fopen (prj_file, "w")) == NULL)
                                {
                                  QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to create ESRI PRJ file ") +
                                                         QDir::toNativeSeparators (QString (prj_file)) + 
                                                         tr ("  The error message returned was:\n\n") +
                                                         QString (strerror (errno)));

                                  exit (-1);
                                }

                              fprintf (prj_fp, "COMPD_CS[\"WGS84 with WGS84E Z\",GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],AUTHORITY[\"EPSG\",\"4326\"]],VERT_CS[\"ellipsoid Z in meters\",VERT_DATUM[\"Ellipsoid\",2002],UNIT[\"metre\",1],AXIS[\"Z\",UP]]]\n");
                              fclose (prj_fp);

                              break;
                            }


                          extractList->addItem (tr ("FILE : ") + QString (out_file));
                        }
                    }
                }
            }
        }


      //  For ASCII grid output we have to add a carriage return.

      if (options.format == CH2EX_ASCII_GRID) fprintf (fp, "\n");


      progress.ebar->setValue (row - y_start);
      qApp->processEvents ();
    }

  progress.ebar->setValue (height);
  qApp->processEvents ();


  chrtr2_close_file (chrtr2_handle);


  switch (options.format)
    {
    case CH2EX_ASCII:
      string.sprintf (tr ("%d ASCII records output").toLatin1 (), out_count);
      extractList->addItem (string);
      fclose (fp);
      break;

    case CH2EX_ASCII_GRID:
      string.sprintf (tr ("%d ASCII grid records output").toLatin1 (), out_count);
      extractList->addItem (string);
      fclose (fp);

      if (options.size)
        {
          sprintf (out_file, "%s.%02d.grd", orig, file_count);
        }
      else
        {
          sprintf (out_file, "%s.grd", orig);
        }

      if ((fp = fopen64 (out_file, "r+")) == NULL)
	{
          QMessageBox::critical (this, tr ("chrtr2Extract"), tr ("Unable to open output file:") + QString (out_file) +
                                 tr ("\nReason:") + QString (strerror (errno)));
	  exit (-1);
	}

      fprintf (fp, "%14.9f %14.9f %14.9f %14.9f %14.9f %14.9f %05d %05d %15.2f %15.2f\n", mbr.wlon, mbr.elon, mbr.slat, mbr.nlat,
               chrtr2_header.lon_grid_size_degrees, chrtr2_header.lat_grid_size_degrees, width, height, min_z, max_z);
      fclose (fp);
      break;

    case CH2EX_LLZ:
      string.sprintf (tr ("%d LLZ records output").toLatin1 (), out_count);
      extractList->addItem (string);
      close_llz (llz_hnd);
      break;

    case CH2EX_RDP:
      string.sprintf (tr ("%d RDP records output").toLatin1 (), out_count);
      extractList->addItem (string);
      fclose (fp);
      break;

    case CH2EX_SHP:
      string.sprintf (tr ("%d SHP records output").toLatin1 (), out_count);
      extractList->addItem (string);
      SHPClose (shp_hnd);
      DBFClose (dbf_hnd);  
      break;

    case CH2EX_CHRTR2:
      string.sprintf (tr ("%d CHRTR2 records output").toLatin1 (), out_count);
      extractList->addItem (string);

      out_header.min_observed_z = min_z;
      out_header.max_observed_z = max_z;

      chrtr2_update_header (out_handle, out_header);

      chrtr2_close_file (out_handle);
      break;
    }

  qApp->processEvents ();


  button (QWizard::FinishButton)->setEnabled (true);
  button (QWizard::CancelButton)->setEnabled (false);


  QApplication::restoreOverrideCursor ();


  extractList->addItem (" ");
  QListWidgetItem *cur = new QListWidgetItem (tr ("Extraction complete, press Finish to exit."));

  extractList->addItem (cur);
  extractList->setCurrentItem (cur);
  extractList->scrollToItem (cur);
}
