// libstdaudio
// Copyright (c) 2018 - Timur Doumler

#pragma once
#include <memory>
#include <functional>
#include <string_view>
#include "config.h"
#include "buffer_list.h"

LIBSTDAUDIO_NAMESPACE_BEGIN

class _device_impl;

/** A handle to an audio input and/or output device. */
class device {
public:
  /** Default constructor.
   *  Constructs a null device, i.e. a device with no input and output channels.
   */
  device();

  /** Move constructor. */
  device(device&&) noexcept;

  /** Destructor. */
  ~device();

  /** Returns the name of the device. If the device is a null-device, this is
   *  an empty string.
   */
  string_view name() const;

  /** Returns true if the device provides input buffers. */
  bool is_input() const noexcept;

  /** Returns true if the device provides output buffers. */
  bool is_output() const noexcept;

  /** Audio callback type. */
  using callback = function<void(device&, buffer_list&)>;

  /** Connect a callback to the device. Disconnect any previous callbacks.
   *  If the device is not polling, the callback will be automatically
   *  called on the audio thread.
   */
  void connect(callback);

  /** If the device is polling, returns true. Otherwise, returns false. */
  bool is_polling() const noexcept;

  /** If the device is polling, blocks until there is a new processing
   *  buffer available. Otherwise, does nothing.
   */
  void wait();

  /** If the device is polling, calls the callback once on the current
   *  thread, passing in the current processing buffer. Otherwise, does
   *  nothing.
   */
  void process();

private:
  unique_ptr<_device_impl> _impl;
  template <typename Impl, typename... Args>  friend device _make_device_with_impl(Args&&...);
};

/** Returns the current default audio input device. */
device get_input_device();

/** Returns the current default audio output device. */
device get_output_device();

LIBSTDAUDIO_NAMESPACE_END