#ifndef FIREBASE_HTTP_CLIENT_H
#define FIREBASE_HTTP_CLIENT_H

#include <string>

#include "Arduino.h"
#include "Stream.h"

struct HttpStatus
{
  static const int TEMPORARY_REDIRECT = 307;
};

class FirebaseHttpClient
{
public:
  static FirebaseHttpClient *create();

  virtual void setReuseConnection(bool reuse) = 0;
  virtual void begin(const std::string &url) = 0;
  virtual void begin(const std::string &host, const std::string &path) = 0;

  virtual void end() = 0;

  virtual void addHeader(const std::string &name, const std::string &value) = 0;
  virtual void collectHeaders(const char *header_keys[],
                              const int header_key_count) = 0;
  virtual std::string header(const std::string &name) = 0;

  virtual int sendRequest(const std::string &method, const std::string &data) = 0;

  virtual std::string getString() = 0;

  virtual Stream *getStreamPtr() = 0;

  virtual std::string errorToString(int error_code) = 0;

  virtual bool connected() = 0;

protected:
  static const uint16_t kFirebasePort = 443;
};

static const char kFirebaseFingerprint[] =
    "6A:7D:A1:11:7F:F3:C2:45:0C:F7:22:E7:A2:35:EA:61:12:55:83:48"; // 2020-02

#endif // FIREBASE_HTTP_CLIENT_H
