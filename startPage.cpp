
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



#include "startPage.hpp"
#include "startPageHelp.hpp"

startPage::startPage (int32_t *argc, char **argv, QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/chrtr2ExtractWatermark.png"));


  setTitle (tr ("Introduction"));

  setWhatsThis (tr ("See, it really works!"));

  QLabel *label = new QLabel (tr ("The chrtr2Extract program is a tool for reading a CHRTR2 file and writing out a series of "
                                  "files (in ASCII, LLZ, RDP, or ESRI Shape) for input to CARIS or other software packages.  Help is "
                                  "available by clicking on the Help button and then clicking on the item for which you "
                                  "want help.  Click <b>Next</b> to continue or <b>Cancel</b> to exit."));


  label->setWordWrap(true);


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->addWidget (label);
  vbox->addStretch (10);


  QHBoxLayout *chrtr2_file_box = new QHBoxLayout (0);
  chrtr2_file_box->setSpacing (8);

  vbox->addLayout (chrtr2_file_box);


  QLabel *chrtr2_file_label = new QLabel (tr ("CHRTR2 File"), this);
  chrtr2_file_box->addWidget (chrtr2_file_label, 1);

  chrtr2_file_edit = new QLineEdit (this);
  chrtr2_file_edit->setReadOnly (true);
  chrtr2_file_box->addWidget (chrtr2_file_edit, 10);

  QPushButton *chrtr2_file_browse = new QPushButton (tr ("Browse..."), this);
  chrtr2_file_box->addWidget (chrtr2_file_browse, 1);

  chrtr2_file_label->setWhatsThis (chrtr2_fileText);
  chrtr2_file_edit->setWhatsThis (chrtr2_fileText);
  chrtr2_file_browse->setWhatsThis (chrtr2_fileBrowseText);

  connect (chrtr2_file_browse, SIGNAL (clicked ()), this, SLOT (slotCHRTR2FileBrowse ()));



  QHBoxLayout *output_box = new QHBoxLayout (0);
  output_box->setSpacing (8);

  vbox->addLayout (output_box);


  QLabel *output_file_label = new QLabel (tr ("Output file base name"), this);
  output_box->addWidget (output_file_label, 1);

  output_file_edit = new QLineEdit (this);
  output_box->addWidget (output_file_edit, 10);

  output_file_label->setWhatsThis (output_fileText);
  output_file_edit->setWhatsThis (output_fileText);


  QGroupBox *areaBox = new QGroupBox (tr ("Optional area"), this);
  QHBoxLayout *areaBoxLayout = new QHBoxLayout;
  areaBox->setLayout (areaBoxLayout);
  areaBoxLayout->setSpacing (10);

  area_file_edit = new QLineEdit (this);
  area_file_edit->setReadOnly (true);
  area_file_edit->setToolTip (tr ("Area file name")); 
  area_file_edit->setWhatsThis (area_fileText);
  areaBoxLayout->addWidget (area_file_edit);

  QPushButton *area_browse = new QPushButton (tr ("Browse..."), this);
  area_browse->setToolTip (tr ("Select an area file to define the extraction area"));
  area_browse->setWhatsThis (area_fileBrowseText);
  connect (area_browse, SIGNAL (clicked ()), this, SLOT (slotAreaFileBrowse ()));
  areaBoxLayout->addWidget (area_browse);

  QPushButton *area_nsew = new QPushButton (tr ("NSEW..."), this);
  area_nsew->setToolTip (tr ("Create an area file by defining North, South, East, and West bounds")); 
  area_nsew->setWhatsThis (tr ("Create an area file by defining North, South, East, and West bounds"));
  connect (area_nsew, SIGNAL (clicked ()), this, SLOT (slotAreaNSEW ()));
  areaBoxLayout->addWidget (area_nsew);

  vbox->addWidget (areaBox, 1);


  if (*argc == 2)
    {
      CHRTR2_HEADER chrtr2_header;
      char in_file[512];
      int32_t chrtr2_handle;

      QString chrtr2_file_name = QString (argv[1]);

      strcpy (in_file, chrtr2_file_name.toLatin1 ());

      if ((chrtr2_handle = chrtr2_open_file (in_file, &chrtr2_header, CHRTR2_READONLY)) < 0)
        {
          QMessageBox::warning (this, tr ("chrtr2Extract error"), tr ("The file ") + QDir::toNativeSeparators (QString (in_file)) + 
                                tr (" is not a CHRTR2 file or there was an error reading the file.") + tr ("  The error message returned was:\n\n") +
                                QString (chrtr2_strerror ()));
        }
      else
        {
          chrtr2_file_edit->setText (chrtr2_file_name);

          output_file_edit->setText (chrtr2_file_name);

          chrtr2_close_file (chrtr2_handle);
        }
    }


  if (!chrtr2_file_edit->text ().isEmpty ())
    {
      registerField ("chrtr2_file_edit", chrtr2_file_edit);
    }
  else
    {
      registerField ("chrtr2_file_edit*", chrtr2_file_edit);
    }


  registerField ("output_file_edit", output_file_edit);
  registerField ("area_file_edit", area_file_edit);
}



void startPage::slotCHRTR2FileBrowse ()
{
  QStringList         files, filters;
  QString             file;


  QFileDialog *fd = new QFileDialog (this, tr ("chrtr2Extract Open CHRTR2 File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->input_dir);


  filters << tr ("CHRTR2 (*.ch2)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("CHRTR2 (*.ch2)"));

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString chrtr2_file_name = files.at (0);


      if (!chrtr2_file_name.isEmpty())
        {
          CHRTR2_HEADER chrtr2_header;
          char in_file[512];
          int32_t chrtr2_handle;

          strcpy (in_file, chrtr2_file_name.toLatin1 ());

          if ((chrtr2_handle = chrtr2_open_file (in_file, &chrtr2_header, CHRTR2_READONLY)) < 0)
            {
              QMessageBox::warning (this, tr ("chrtr2Extract error"), tr ("The file ") + QDir::toNativeSeparators (QString (in_file)) + 
                                    tr (" is not a CHRTR2 file or there was an error reading the file.") + tr ("  The error message returned was:\n\n") +
                                    QString (chrtr2_strerror ()));

              return;
            }


          chrtr2_close_file (chrtr2_handle);

          chrtr2_file_edit->setText (chrtr2_file_name);

          output_file_edit->setText (chrtr2_file_name);
        }

      options->input_dir = fd->directory ().absolutePath ();
    }
}



void startPage::slotAreaFileBrowse ()
{
  SHPHandle shpHandle;
  SHPObject *shape = NULL;
  int32_t type, numShapes;
  double minBounds[4], maxBounds[4];


  QFileDialog *fd = new QFileDialog (this, tr ("chrtr2Extract Area File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->area_dir);


  QStringList filters;
  filters << tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString area_file_name = files.at (0);


      if (!area_file_name.isEmpty())
        {
          if (area_file_name.endsWith (".shp", Qt::CaseInsensitive))
            {
              char shpname[1024];
              strcpy (shpname, area_file_name.toLatin1 ());


              //  Open shape file

              shpHandle = SHPOpen (shpname, "rb");

              if (shpHandle == NULL)
                {
                  QMessageBox::warning (this, tr ("chrtr2Extract"), tr ("Cannot open shape file %1!").arg (area_file_name));
                  return;
                }
              else
                {
                  //  Get shape file header info

                  SHPGetInfo (shpHandle, &numShapes, &type, minBounds, maxBounds);


                  if (type != SHPT_POLYGON && type != SHPT_POLYGONZ && type != SHPT_POLYGONM &&
                      type != SHPT_ARC && type != SHPT_ARCZ && type != SHPT_ARCM)
                    {
                      QMessageBox::warning (this, tr ("chrtr2Extract"), tr ("Shape file %1 is not a polygon or polyline file!").arg (area_file_name));
                      return;
                    }
                  else
                    {
                      //  Read only the first shape.

                      shape = SHPReadObject (shpHandle, 0);


                      //  Check the number of vertices.

                      if (shape->nVertices < 3)
                        {
                          SHPClose (shpHandle);
                          QMessageBox::warning (this, tr ("chrtr2Extract"), tr ("Number of vertices (%1) of shape file %2 is too few for a polygon!").arg
                                                (shape->nVertices).arg (area_file_name));
                          return;
                        }


                      //  Read the vertices to take a shot at determining that this is a geographic polygon.

                      for (int32_t j = 0 ; j < shape->nVertices ; j++)
                        {
                          if (shape->padfX[j] < -360.0 || shape->padfX[j] > 360.0 || shape->padfY[j] < -90.0 || shape->padfY[j] > 90.0)
                            {
                              SHPDestroyObject (shape);
                              SHPClose (shpHandle);
                              QMessageBox::warning (this, tr ("chrtr2Extract"), tr ("Shape file %1 does not appear to be geographic!").arg (area_file_name));
                              return;
                            }
                        }


                      SHPDestroyObject (shape);
                      SHPClose (shpHandle);
                    }
                }
            }

          area_file_edit->setText (area_file_name);
        }

      options->area_dir = fd->directory ().absolutePath ();
    }
}



void 
startPage::slotAreaNSEW ()
{
  if (output_file_edit->text ().isEmpty ())
    {
      QMessageBox::warning (this, tr ("chrtr2Extract"),
                            tr ("You must first select a CHRTR2 input file before trying to define an optional extraction area."));
    }
  else
    {
      nsewD = new QDialog (this, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
      nsewD->setWindowTitle (tr ("pfmLoad NSEW bounds"));
      nsewD->setModal (true);


      QVBoxLayout *nsewBox = new QVBoxLayout (nsewD);
      nsewBox->setMargin (5);
      nsewBox->setSpacing (5);


      QGroupBox *northBox = new QGroupBox (tr ("North Latitude"), nsewD);
      QHBoxLayout *northBoxLayout = new QHBoxLayout;
      northBox->setLayout (northBoxLayout);
      northBoxLayout->setSpacing (10);

      north = new QLineEdit (nsewD);
      north->setToolTip (tr ("Set the northern boundary of the area"));
      north->setWhatsThis (tr ("Set the northern boundary of the area"));

      northBoxLayout->addWidget (north, 1);

      nsewBox->addWidget (northBox);


      QGroupBox *southBox = new QGroupBox (tr ("South Latitude"), nsewD);
      QHBoxLayout *southBoxLayout = new QHBoxLayout;
      southBox->setLayout (southBoxLayout);
      southBoxLayout->setSpacing (10);

      south = new QLineEdit (nsewD);
      south->setToolTip (tr ("Set the southern boundary of the area"));
      south->setWhatsThis (tr ("Set the southern boundary of the area"));

      southBoxLayout->addWidget (south, 1);

      nsewBox->addWidget (southBox);


      QGroupBox *westBox = new QGroupBox (tr ("West Longitude"), nsewD);
      QHBoxLayout *westBoxLayout = new QHBoxLayout;
      westBox->setLayout (westBoxLayout);
      westBoxLayout->setSpacing (10);

      west = new QLineEdit (nsewD);
      west->setToolTip (tr ("Set the western boundary of the area"));
      west->setWhatsThis (tr ("Set the western boundary of the area"));

      westBoxLayout->addWidget (west, 1);

      nsewBox->addWidget (westBox);


      QGroupBox *eastBox = new QGroupBox (tr ("East Longitude"), nsewD);
      QHBoxLayout *eastBoxLayout = new QHBoxLayout;
      eastBox->setLayout (eastBoxLayout);
      eastBoxLayout->setSpacing (10);

      east = new QLineEdit (nsewD);
      east->setToolTip (tr ("Set the eastern boundary of the area"));
      east->setWhatsThis (tr ("Set the eastern boundary of the area"));

      eastBoxLayout->addWidget (east, 1);

      nsewBox->addWidget (eastBox);


      QHBoxLayout *actions = new QHBoxLayout (0);
      nsewBox->addLayout (actions);

      QPushButton *bHelp = new QPushButton (nsewD);
      bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
      bHelp->setToolTip (tr ("Enter What's This mode for help"));
      connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
      actions->addWidget (bHelp);

      actions->addStretch (10);


      QPushButton *applyButton = new QPushButton (tr ("OK"), nsewD);
      applyButton->setToolTip (tr ("Accept values and close dialog"));
      connect (applyButton, SIGNAL (clicked ()), this, SLOT (slotApplyNSEW ()));
      actions->addWidget (applyButton);

      QPushButton *closeButton = new QPushButton (tr ("Cancel"), nsewD);
      closeButton->setToolTip (tr ("Discard values and close dialog"));
      connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseNSEW ()));
      actions->addWidget (closeButton);


      nsewD->show ();
    }
}



void
startPage::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void 
startPage::slotApplyNSEW ()
{
  if (north->text ().isEmpty () || south->text ().isEmpty () || west->text ().isEmpty () || east->text ().isEmpty ())
    {
      QMessageBox::warning (this, tr ("chrtr2Extract NSEW bounds"), tr ("You must set all four fields or use the Cancel button."));
    }
  else
    {
      char nlat[50], slat[50], elon[50], wlon[50], file[512], hem;
      double ndegs, sdegs, wdegs, edegs, deg, min, sec, tmp;

      strcpy (nlat, north->text ().toLatin1 ());
      strcpy (slat, south->text ().toLatin1 ());
      strcpy (elon, east->text ().toLatin1 ());
      strcpy (wlon, west->text ().toLatin1 ());

      posfix (nlat, &ndegs, POS_LAT);
      posfix (slat, &sdegs, POS_LAT);
      posfix (elon, &edegs, POS_LON);
      posfix (wlon, &wdegs, POS_LON);

      if (ndegs < sdegs)
	{
	  tmp = sdegs;
	  sdegs = ndegs;
	  ndegs = tmp;
	}

      if (edegs < wdegs)
	{
	  if ((edegs < 0.0 && wdegs < 0.0) || (edegs >= 0.0 && wdegs >= 0.0))
	    {
	      tmp = wdegs;
	      wdegs = edegs;
	      edegs = tmp;
	    }
	}


      QString area_file_name = output_file_edit->text () + ".are";

      strcpy (file, area_file_name.toLatin1 ());

      FILE *fp;
      
      if ((fp = fopen (file, "w")) == NULL)
	{
	  QMessageBox::warning (this, tr ("chrtr2Extract NSEW bounds"), tr ("Unable to open area file ") + area_file_name);
	}
      else
	{
	  strcpy (nlat, fixpos (ndegs, &deg, &min, &sec, &hem, POS_LAT, POS_HDMS));
	  strcpy (slat, fixpos (sdegs, &deg, &min, &sec, &hem, POS_LAT, POS_HDMS));
	  strcpy (wlon, fixpos (wdegs, &deg, &min, &sec, &hem, POS_LON, POS_HDMS));
	  strcpy (elon, fixpos (edegs, &deg, &min, &sec, &hem, POS_LON, POS_HDMS));

	  fprintf (fp, "%s, %s\n", slat, wlon);
	  fprintf (fp, "%s, %s\n", nlat, wlon);
	  fprintf (fp, "%s, %s\n", nlat, elon);
	  fprintf (fp, "%s, %s\n", slat, elon);

	  fclose (fp);

          area_file_edit->setText (area_file_name);
	}

      nsewD->close ();
    }
}



void 
startPage::slotCloseNSEW ()
{
  nsewD->close ();
}
