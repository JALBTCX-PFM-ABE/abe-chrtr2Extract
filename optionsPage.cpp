
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



#include "optionsPage.hpp"
#include "optionsPageHelp.hpp"

optionsPage::optionsPage (QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setTitle (tr ("Extract Options"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/chrtr2ExtractWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  QGroupBox *uBox = new QGroupBox (tr ("Output units"), this);
  QHBoxLayout *uBoxLayout = new QHBoxLayout;
  uBox->setLayout (uBoxLayout);

  units = new QComboBox (this);
  units->setToolTip (tr ("Set the output file units"));
  units->setWhatsThis (unitsText);
  units->setEditable (false);
  units->addItem (tr ("Meters"));
  units->addItem (tr ("Feet"));

  //  SJ - per instructed, I have removed the CHRTR2_FATHOMS option to limit confusion of users
  //  units->addItem (tr ("Fathoms"));

  units->addItem (tr ("Fathoms @ 4800 ft/sec"));
  units->setCurrentIndex (options->z_units);
  connect (units, SIGNAL (currentIndexChanged (int)), this, SLOT (slotUnitsChanged (int)));
  uBoxLayout->addWidget (units);


  vbox->addWidget (uBox);


  QGroupBox *tBox = new QGroupBox (tr ("Options"), this);
  QVBoxLayout *tBoxLayout = new QVBoxLayout;
  QHBoxLayout *tBoxTopLayout = new QHBoxLayout;
  QHBoxLayout *tBoxBotLayout = new QHBoxLayout;
  tBoxLayout->addLayout (tBoxTopLayout);
  tBoxLayout->addLayout (tBoxBotLayout);
  tBox->setLayout (tBoxLayout);
  tBoxLayout->setSpacing (10);

  QGroupBox *oBox = new QGroupBox (tr ("Output format"), this);
  QHBoxLayout *oBoxLayout = new QHBoxLayout;
  oBox->setLayout (oBoxLayout);
  oBoxLayout->setSpacing (10);

  format = new QComboBox (this);
  format->setToolTip (tr ("Set the output file format"));
  format->setWhatsThis (formatText);
  format->setEditable (false);
  format->addItem (tr ("ASCII"));
  format->addItem (tr ("ASCII grid"));
  format->addItem (tr ("LLZ"));
  format->addItem (tr ("RDP"));
  format->addItem (tr ("SHP"));
  format->addItem (tr ("CHRTR2"));
  format->setCurrentIndex (options->format);
  connect (format, SIGNAL (currentIndexChanged (int)), this, SLOT (slotFormatChanged (int)));
  oBoxLayout->addWidget (format);
  tBoxTopLayout->addWidget (oBox);


  QGroupBox *fbox = new QGroupBox (tr ("File size"), this);
  QHBoxLayout *fboxLayout = new QHBoxLayout;
  fbox->setLayout (fboxLayout);
  fboxLayout->setSpacing (10);

  size = new QComboBox (this);
  size->setToolTip (tr ("Set the output file size limit"));
  size->setWhatsThis (sizeText);
  size->setEditable (false);
  size->addItem (tr ("No limit"));
  size->addItem (tr ("1GB"));
  size->addItem (tr ("2GB"));
  size->addItem (tr ("4GB"));
  size->setCurrentIndex (options->size);
  fboxLayout->addWidget (size);
  tBoxTopLayout->addWidget (fbox);



  QGroupBox *cutoffBox = new QGroupBox (tr ("Depth cutoff"), this);
  QHBoxLayout *cutoffBoxLayout = new QHBoxLayout;
  cutoffBox->setLayout (cutoffBoxLayout);

  cut = new QCheckBox (tr ("Apply cutoff"), this);
  cut->setToolTip (tr ("Set this to apply the depth cutoff"));
  cut->setWhatsThis (cutText);
  cut->setChecked (options->cut);
  connect (cut, SIGNAL (stateChanged (int)), this, SLOT (slotCutChanged (int)));
  cutoffBoxLayout->addWidget (cut);

  cutoff = new QDoubleSpinBox (cutoffBox);
  cutoff->setDecimals (2);
  cutoff->setValue (options->cutoff);
  cutoff->setWrapping (false);
  cutoff->setWhatsThis (cutoffText);
  if (!options->cut) cutoff->setEnabled (false);
  cutoffBoxLayout->addWidget (cutoff);
  tBoxTopLayout->addWidget (cutoffBox);


  QGroupBox *shiftBox = new QGroupBox (tr ("Datum shift"), this);
  QHBoxLayout *shiftBoxLayout = new QHBoxLayout;
  shiftBox->setLayout (shiftBoxLayout);
  datumShift = new QDoubleSpinBox (shiftBox);
  datumShift->setDecimals (2);
  datumShift->setValue (options->datum_shift);
  datumShift->setWrapping (false);
  datumShift->setWhatsThis (datumShiftText);
  shiftBoxLayout->addWidget (datumShift);
  tBoxTopLayout->addWidget (shiftBox);


  QGroupBox *chkBox = new QGroupBox (tr ("Checked data"), this);
  QHBoxLayout *chkBoxLayout = new QHBoxLayout;
  chkBox->setLayout (chkBoxLayout);
  checked = new QCheckBox (this);
  checked->setToolTip (tr ("Only include data from checked bins"));
  checked->setWhatsThis (checkedText);
  checked->setChecked (options->chk);
  chkBoxLayout->addWidget (checked);
  tBoxBotLayout->addWidget (chkBox);


  QGroupBox *flpBox = new QGroupBox (tr ("Invert sign"), this);
  QHBoxLayout *flpBoxLayout = new QHBoxLayout;
  flpBox->setLayout (flpBoxLayout);
  flip = new QCheckBox (this);
  flip->setToolTip (tr ("Invert the sign on Z values"));
  flip->setWhatsThis (flipText);
  flip->setChecked (options->flp);
  flpBoxLayout->addWidget (flip);
  tBoxBotLayout->addWidget (flpBox);


  utmBox = new QGroupBox (tr ("UTM"), this);
  QHBoxLayout *utmBoxLayout = new QHBoxLayout;
  utmBox->setLayout (utmBoxLayout);
  utm = new QCheckBox (this);
  utm->setToolTip (tr ("Convert position data to UTM"));
  utm->setWhatsThis (utmText);
  utm->setChecked (options->utm);
  utmBoxLayout->addWidget (utm);
  tBoxBotLayout->addWidget (utmBox);
  if (options->format)
    {
      utm->setEnabled (false);
      utmBox->setEnabled (false);
    }


  uncBox = new QGroupBox (tr ("Include uncertainty data"), this);
  QHBoxLayout *uncBoxLayout = new QHBoxLayout;
  uncBox->setLayout (uncBoxLayout);
  uncert = new QCheckBox (this);
  uncert->setToolTip (tr ("Include uncertainty data in output"));
  uncert->setWhatsThis (uncertText);
  uncert->setChecked (options->unc);
  uncBoxLayout->addWidget (uncert);
  tBoxBotLayout->addWidget (uncBox);
  if (options->format)
    {
      uncert->setEnabled (false);
      uncBox->setEnabled (false);
    }


  vbox->addWidget (tBox);


  slotUnitsChanged (options->z_units);


  registerField ("format", format);
  registerField ("checked", checked);
  registerField ("flip", flip);
  registerField ("utm", utm);
  registerField ("uncert", uncert);
  registerField ("cut", cut);
  registerField ("cutoff", cutoff, "value", "valueChanged");
  registerField ("datumShift", datumShift, "value", "valueChanged");
  registerField ("size", size);
}



void 
optionsPage::slotUnitsChanged (int id)
{
  options->z_units = id;

  switch (options->z_units)
    {
    case CHRTR2_METERS:
      cutoff->setRange (-12000.0, 9000.0);
      cutoff->setSingleStep (100.0);
      cutoff->setToolTip (tr ("Set a depth cutoff value in meters"));
      datumShift->setRange (-2000.0, 2000.0);
      datumShift->setSingleStep (10.0);
      datumShift->setToolTip (tr ("Set a value, in meters, to add to the data"));
      break;

    case CHRTR2_FEET:
      cutoff->setRange (-30000.0, 37000.0);
      cutoff->setSingleStep (1000.0);
      cutoff->setToolTip (tr ("Set a depth cutoff value in feet"));
      datumShift->setRange (-7000.0, 7000.0);
      datumShift->setSingleStep (100.0);
      datumShift->setToolTip (tr ("Set a value, in feet, to add to the data"));
      break;

    case CHRTR2_FATHOMS:
      cutoff->setRange (-5000.0, 7000.0);
      cutoff->setSingleStep (10.0);
      cutoff->setToolTip (tr ("Set a depth cutoff value in fathoms"));
      datumShift->setRange (-1100.0, 1100.0);
      datumShift->setSingleStep (100.0);
      datumShift->setToolTip (tr ("Set a value, in fathoms, to add to the data"));
      break;

    case CHRTR2_FATHOMS_AT_4800:
      cutoff->setRange (-5000.0, 7000.0);
      cutoff->setSingleStep (10.0);
      cutoff->setToolTip (tr ("Set a depth cutoff value in fathoms (@ 4800 ft/sec"));
      datumShift->setRange (-1100.0, 1100.0);
      datumShift->setSingleStep (100.0);
      datumShift->setToolTip (tr ("Set a value, in fathoms with sound velocity at 4800 feet per second, to add to the data"));
      break;
    }
}



void 
optionsPage::slotCutChanged (int state)
{
  if (state)
    {
      cutoff->setEnabled (true);
    }
  else
    {
      cutoff->setEnabled (false);
    }
}



void 
optionsPage::slotFormatChanged (int id)
{
  if (id)
    {
      options->utm = NVFalse;
      utm->setChecked (false);
      utm->setEnabled (false);
      utmBox->setEnabled (false);

      options->unc = NVFalse;
      uncert->setChecked (false);
      uncert->setEnabled (false);
      uncBox->setEnabled (false);
    }
  else
    {
      utm->setEnabled (true);
      utmBox->setEnabled (true);

      uncert->setEnabled (true);
      uncBox->setEnabled (true);


      //  For CHRTR2 we force the units to METERS.

      if (id == CH2EX_CHRTR2) units->setCurrentIndex (CHRTR2_METERS);
    }
}



void
optionsPage::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}
