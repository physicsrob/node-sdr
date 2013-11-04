#!/usr/bin/env python
# Derived from quisk
import sys
import usb.core, usb.util
import struct, threading, time, traceback, math

si570_xtal_freq = 114285000

usb_vendor_id = 0x16c0
usb_product_id = 0x05dc
si570_i2c_address = 0x55

IN =  usb.util.build_request_type(usb.util.CTRL_IN,  usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_DEVICE)
OUT = usb.util.build_request_type(usb.util.CTRL_OUT, usb.util.CTRL_TYPE_VENDOR, usb.util.CTRL_RECIPIENT_DEVICE)
UBYTE2 = struct.Struct('<H')
UBYTE4 = struct.Struct('<L')	# Thanks to Sivan Toledo

class Softrock(object):
  def __init__(self):
    self.usb_dev = None
  def open(self):			# Called once to open the Hardware
    # find our device
    self.usb_dev = usb.core.find(idVendor=usb_vendor_id, idProduct=usb_product_id)
    if self.usb_dev is None:
        raise Exception("Could not find device.")
    self.usb_dev.set_configuration()        # and to David, AE9RB, for the fix.
    ret = self.usb_dev.ctrl_transfer(IN, 0x00, 0x0E00, 0, 2)
  def OnPTT(self, ptt):
    self.usb_dev.ctrl_transfer(IN, 0x50, ptt, 0, 3)
  def GetFreq(self):	# return the running frequency / 4
    if not self.usb_dev:
      return 0
    ret = self.usb_dev.ctrl_transfer(IN, 0x3A, 0, 0, 4)
    s = ret.tostring()
    freq = UBYTE4.unpack(s)[0]
    freq = int(freq * 1.0e6 / 2097152.0 / 4.0 + 0.5)
    return freq
  def SetFreq(self, freq):
    freq = int(freq/1.0e6 * 2097152.0 * 4.0 + 0.5)
    s = UBYTE4.pack(freq)
    try:
      self.usb_dev.ctrl_transfer(OUT, 0x32, si570_i2c_address + 0x700, 0, s)
    except usb.core.USBError:
      if DEBUG: traceback.print_exc()
    else:
      return True

if __name__ == "__main__":
    func = sys.argv[1];
    if func == 'setfreq':
        s=Softrock()
        s.open();
        s.SetFreq(float(sys.argv[2]));
        print "Frequency set to: %f" % ( s.GetFreq() )
    elif func == 'getfreq':
        s=Softrock()
        s.open();
        print s.GetFreq();
    elif func == 'setptt':
        s=Softrock()
        s.open();
        s.OnPTT(sys.argv[2]=='on')
    else:
        print "No such command"

