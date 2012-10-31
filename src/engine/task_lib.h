// The Firmament project
// Copyright (c) 2011-2012 Malte Schwarzkopf <malte.schwarzkopf@cl.cam.ac.uk>
//
// The task library class is part of libfirmament, the library that gets linked
// into task binaries. Eventually, we should make this an interface, and support
// platform-specific classes.

#ifndef FIRMAMENT_ENGINE_TASK_LIB_H
#define FIRMAMENT_ENGINE_TASK_LIB_H

#include <string>
#ifdef __PLATFORM_HAS_BOOST__
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#else
// Currently this won't build if __PLATFORM_HAS_BOOST__ is not defined.
#error __PLATFORM_HAS_BOOST__ not set, so cannot build task library!
#endif

#include "base/common.h"
#include "base/types.h"
#include "base/data_object.h"
#include "base/resource_desc.pb.h"
#include "base/task_interface.h"
#include "messages/base_message.pb.h"
// XXX(malte): include order dependency
#include "platforms/unix/common.h"
#include "misc/messaging_interface.h"
#include "misc/protobuf_envelope.h"
#include "platforms/common.h"
#include "platforms/unix/stream_sockets_adapter.h"
#include "platforms/unix/stream_sockets_adapter-inl.h"
#include "platforms/unix/stream_sockets_channel-inl.h"

namespace firmament {

using platform_unix::streamsockets::StreamSocketsAdapter;
using platform_unix::streamsockets::StreamSocketsChannel;

class TaskLib {
 public:
  explicit TaskLib();
  void Run();
  void AwaitNextMessage();
  bool ConnectToCoordinator(const string& coordinator_uri);
  void RunTask();
  // CIEL programming model
  /*virtual const string Construct(const DataObject& object);
  virtual void Spawn(const ConcreteReference& code,
                     vector<FutureReference>* outputs);
  virtual void Publish(const vector<ConcreteReference>& references);
  virtual void TailSpawn(const ConcreteReference& code);*/

 protected:
  shared_ptr<StreamSocketsAdapter<BaseMessage> > m_adapter_;
  StreamSocketsChannel<BaseMessage> chan_;
  bool exit_;
  // TODO(malte): transform this into a better representation
  string coordinator_uri_;
  ResourceID_t resource_id_;

  void HandleWrite(const boost::system::error_code& error,
                   size_t bytes_transferred);
  bool RegisterWithCoordinator();
  void SendFinalizeMessage(bool success);
  void SendHeartbeat();
  bool SendMessageToCoordinator(BaseMessage* msg);
  void AwaitMessage();

 private:
  bool task_running_;
};

}  // namespace firmament

#endif  // FIRMAMENT_ENGINE_TASK_LIB_H
