#include "Archive.h"
#include "Array.h"
#include "Vector.h"

#include <cstring>
#include <fstream>
#include <iostream>

const int kArchiveMagic = 12221991;
const size_t kStreamThreshold = 1024 * 1024;

namespace {
  void decrypt(char& c, size_t i) {
    c = (char)(c - (i*i + 2*i));
  }

  void encrypt(char& c, size_t i) {
    c = (char)(c + (i*i + 2*i));
  }
}

namespace LTE {
  struct ArchiveBasicImpl : public Archive {
    struct File {
      String name;
      std::streamoff location;
      size_t size;
      char* data;
    };

    std::vector<File> files;
    std::fstream stream;
    String filename;
    bool modified;

    ArchiveBasicImpl(const String& filename) :
      filename(filename),
      modified(false)
    {
      ReadFrom(filename);
    }

    ~ArchiveBasicImpl() {
      stream.close();

      if (modified) {
        std::ofstream writeFile(filename.c_str(), std::ios::binary);
        LTE_ASSERT(writeFile);

        /* Write the header. */
        writeFile.write((const char*)&kArchiveMagic, sizeof(kArchiveMagic));
        int fileCount = files.size();
        writeFile.write((const char*)&fileCount, sizeof(fileCount));

        /* Compute the size of the index. */
        std::streamoff indexSize = writeFile.tellp();
        for (size_t i = 0; i < files.size(); ++i) {
          indexSize += files[i].name.size() + 1;
          indexSize += sizeof(files[i].location);
          indexSize += sizeof(files[i].size);
        }

        /* Mark the location of each file's data. */
        for (size_t i = 0; i < files.size(); ++i) {
          files[i].location = indexSize;
          indexSize += files[i].size;
        }

        /* Write the index. */
        for (size_t i = 0; i < files.size(); ++i) {
          const char* name = files[i].name.c_str();
          writeFile.write(name, files[i].name.size() + 1);
          writeFile.write((const char*)&files[i].location, sizeof(files[i].location));
          writeFile.write((const char*)&files[i].size, sizeof(files[i].size));
        }

        /* Write the file data. */
        for (size_t i = 0; i < files.size(); ++i) {
          for (size_t j = 0; j < files[i].size; ++j)
            encrypt(files[i].data[j], j);
          writeFile.write((const char*)files[i].data, files[i].size);
        }
      }

      for (size_t i = 0; i < files.size(); ++i)
        free(files[i].data);
    }

    void Dump() const {
      std::cout << files.size() << " files:\n";
      for (size_t i = 0; i < files.size(); ++i)
        std::cout << "\t[" << i << "] : "
                  << files[i].name << " @ "
                  << files[i].location << " ("
                  << files[i].data << " @ " << files[i].size << ")\n";
    }

    bool Contains(const String& filename) {
      String lowerFilename = filename;
      ToLower(lowerFilename);

      for (size_t i = 0; i < files.size(); ++i)
        if (files[i].name == lowerFilename)
          return true;
      return false;
    }

    size_t GetFileCount() const {
      return files.size();
    }

    String GetFileName(size_t index) const {
      LTE_ASSERT(index < files.size());
      return files[index].name;
    }

    void LoadFile(File& file) {
      if (!file.data) {
        stream.seekg(file.location);
        file.data = (char*)malloc(file.size);
        stream.read((char*)file.data, file.size);
        for (size_t i = 0; i < file.size; ++i)
          decrypt(file.data[i], i);
      }
    }

    Array<uchar>* Read(const String& filename) {
      String lowerFilename = filename;
      ToLower(lowerFilename);

      for (size_t i = 0; i < files.size(); ++i) {
        if (files[i].name == lowerFilename) {
          LoadFile(files[i]);
          Array<uchar>* arr = new Array<uchar>(files[i].size);
          memcpy(arr->data(), files[i].data, files[i].size);
          if (files[i].size >= kStreamThreshold)
            UnloadFile(files[i]);
          return arr;
        }
      }
      return NULL;
    }

    bool ReadFrom(const String& filename) {
      stream.open(filename.c_str(), std::ios::in | std::ios::binary);

      if (!stream)
        return false;

      int magic;
      if (!stream.read((char*)&magic, sizeof(int)))
        return false;

      LTE_ASSERT(magic == kArchiveMagic);

      int fileCount;
      if (!stream.read((char*)&fileCount, sizeof(int)))
        return false;

      LTE_ASSERT(fileCount >= 0 && fileCount < 1024);

      for (int i = 0; i < fileCount; ++i) {
        File thisFile;
        thisFile.data = NULL;

        if (!std::getline(stream, thisFile.name, '\0'))
          return false;

        if (!stream.read((char*)&thisFile.location, sizeof(thisFile.location)))
          return false;

        if (!stream.read((char*)&thisFile.size, sizeof(thisFile.size)))
          return false;

        ToLower(thisFile.name);
        files.push_back(thisFile);
      }

      return true;
    }

    void ToLower(String& str) {
      for (size_t i = 0; i < str.size(); ++i)
        str[i] = (char)tolower(str[i]);
    }

    void UnloadFile(File& file) {
      if (file.data) {
        free(file.data);
        file.data = NULL;
      }
    }

    bool Write(const String& filename, const Array<uchar>& data) {
      String lowerFilename = filename;
      ToLower(lowerFilename);

      File* thisFile = NULL;
      for (size_t i = 0; i < files.size(); ++i) {
        if (files[i].name == lowerFilename) {
          thisFile = &files[i];
          free(thisFile->data);
          break;
        }
      }

      if (!thisFile) {
        files.push_back(File());
        thisFile = &files.back();
      }

      thisFile->name = lowerFilename;
      thisFile->location = 0;
      thisFile->size = data.size();
      thisFile->data = (char*)malloc(data.size());
      memcpy(thisFile->data, data.data(), data.size());
      modified = true;
      return true;
    }
  };

  Archive* Archive::Open(const String& filename) {
    return new ArchiveBasicImpl(filename);
  }
}
