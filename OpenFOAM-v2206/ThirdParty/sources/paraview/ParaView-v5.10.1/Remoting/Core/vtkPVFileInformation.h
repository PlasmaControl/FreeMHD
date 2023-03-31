/*=========================================================================

  Program:   ParaView
  Module:    vtkPVFileInformation.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPVFileInformation
 * @brief   Information object that can
 * be used to obtain information about a file/directory.
 *
 * vtkPVFileInformation can be used to collect information about file
 * or directory. vtkPVFileInformation can collect information
 * from a vtkPVFileInformationHelper object alone.
 * @sa
 * vtkPVFileInformationHelper
 */

#ifndef vtkPVFileInformation_h
#define vtkPVFileInformation_h

#include "vtkPVInformation.h"
#include "vtkRemotingCoreModule.h" //needed for exports

#include <string> // Needed for std::string

class vtkCollection;
class vtkPVFileInformationSet;
class vtkFileSequenceParser;

class VTKREMOTINGCORE_EXPORT vtkPVFileInformation : public vtkPVInformation
{
public:
  static vtkPVFileInformation* New();
  vtkTypeMacro(vtkPVFileInformation, vtkPVInformation);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Transfer information about a single object into this object.
   * The object must be a vtkPVFileInformationHelper.
   */
  void CopyFromObject(vtkObject* object) override;

  //@{
  /**
   * Manage a serialized version of the information.
   */
  void CopyToStream(vtkClientServerStream*) override;
  void CopyFromStream(const vtkClientServerStream*) override;
  //@}

  enum FileTypes
  {
    INVALID = 0,
    SINGLE_FILE,
    SINGLE_FILE_LINK,
    DIRECTORY,
    DIRECTORY_LINK,
    FILE_GROUP,
    DRIVE,
    NETWORK_ROOT,
    NETWORK_DOMAIN,
    NETWORK_SERVER,
    NETWORK_SHARE,
    DIRECTORY_GROUP
  };

  //@{
  /**
   * Helper that returns whether a FileType is a
   * directory (DIRECTORY, DRIVE, NETWORK_ROOT, etc...)
   * Or in other words, a type that we can do a DirectoryListing on.
   */
  static bool IsDirectory(int t);
  bool IsDirectory() const { return vtkPVFileInformation::IsDirectory(this->Type); }
  //@}

  //@{
  /**
   * Helper that returns true if file-type is a network related file i.e.
   * either a FILE_GROUP or a DIRECTORY_GROUP.
   */
  static bool IsGroup(int type)
  {
    switch (type)
    {
      case FILE_GROUP:
      case DIRECTORY_GROUP:
        return true;
      default:
        return false;
    }
  }
  bool IsGroup() const { return vtkPVFileInformation::IsGroup(this->Type); }
  //@}

  /**
   * Initializes the information object.
   */
  void Initialize();

  //@{
  /**
   * Get the name of the file/directory whose information is
   * represented by this object.
   */
  vtkGetStringMacro(Name);
  //@}

  //@{
  /**
   * Get the full path of the file/directory whose information is
   * represented by this object.
   */
  vtkGetStringMacro(FullPath);
  //@}

  //@{
  /**
   * Get the type of this file object.
   */
  vtkGetMacro(Type, int);
  //@}

  //@{
  /**
   * Get the state of the hidden flag for the file/directory.
   */
  vtkGetMacro(Hidden, bool);
  //@}

  ///@{
  /**
   * Groups file sequences when found. A file sequence could be [foo1.png, foo2.png, foo6.png].
   * When turned on, the series of files is grouped and our sequence example is named foo..png
   * and original files are discarded from the listing.
   * By default, this flag is set to true.
   */
  vtkGetMacro(GroupFileSequences, bool);
  vtkSetMacro(GroupFileSequences, bool);
  vtkBooleanMacro(GroupFileSequences, bool);
  ///@}

  //@{
  /**
   * Get the Contents for this directory.
   * Returns a collection with vtkPVFileInformation objects
   * for the contents of this directory if Type == DIRECTORY
   * or the contents of this file group if Type == FILE_GROUP
   * or the contents of this directory group if Type == DIRECTORY_GROUP.
   */
  vtkGetObjectMacro(Contents, vtkCollection);
  vtkGetStringMacro(Extension);
  vtkGetMacro(Size, long long);
  vtkGetMacro(ModificationTime, time_t);
  //@}

  /**
   * Fetch the directory listing to be able to use GetSize or GetContents with directories
   */
  void FetchDirectoryListing();

  /**
   * Returns the path to the base data directory path holding various files
   * packaged with ParaView.
   */
  static std::string GetParaViewSharedResourcesDirectory();

  /**
   * Return the path of the example data packaged with ParaView.
   */
  static std::string GetParaViewExampleFilesDirectory();

  /**
   * Return the path of the documents packaged with ParaView.
   */
  static std::string GetParaViewDocDirectory();

protected:
  vtkPVFileInformation();
  ~vtkPVFileInformation() override;

  vtkCollection* Contents;
  vtkFileSequenceParser* SequenceParser;

  char* Name;              // Name of this file/directory.
  char* FullPath;          // Full path for this file/directory.
  int Type;                // Type i.e. File/Directory/FileGroup.
  bool Hidden;             // If file/directory is hidden
  char* Extension;         // File extension
  long long Size;          // File size
  time_t ModificationTime; // File modification time

  vtkSetStringMacro(Extension);
  vtkSetStringMacro(Name);
  vtkSetStringMacro(FullPath);

  void FetchWindowsDirectoryListing();
  void FetchUnixDirectoryListing();

  // Goes thru the collection of vtkPVFileInformation objects
  // are creates file groups, if possible.
  void OrganizeCollection(vtkPVFileInformationSet& vector);

  bool DetectType();
  void GetSpecialDirectories();
  void SetHiddenFlag();
  int FastFileTypeDetection;
  bool ReadDetailedFileInformation;
  bool GroupFileSequences;

private:
  vtkPVFileInformation(const vtkPVFileInformation&) = delete;
  void operator=(const vtkPVFileInformation&) = delete;

  struct vtkInfo;
};

#endif
