#!/usr/bin/python

import usb.core
import usb.util
import sys
import time
from Utils import Utils


class KinectMotor:
    """Kinect class for controling camera's motor"""
    def __init__(self, kIdVendor, kIdProduct):
        self.kIdVendor, kIdProduct = kIdVendor, kIdProduct
        # find our device
        self.dev = usb.core.find(idVendor=kIdVendor, idProduct=kIdProduct)
        if self.dev is None:
            raise ValueError('Device not found')

    def configure(self):
        """configure usb connection"""
        for cfg in self.dev:
            sys.stdout.write("Configuration #"+str(cfg.bConfigurationValue) + '\n')
            for intf in cfg:
                sys.stdout.write('\tInterface #' + \
                                 str(intf.bInterfaceNumber) + \
                                 '\t, Alternate setting ' + \
                                 str(intf.bAlternateSetting) + \
                                 '\n')
                sys.stdout.write("\tEndpoints:\n")
                for ep in intf:
                    sys.stdout.write('\t\t' + \
                                     str(ep.bEndpointAddress) + \
                                     '\n')
        # set the active configuration. With no arguments, the first
        # configuration will be the active one
        self.dev.set_configuration()

    def testRead(self):
        """Send different values and try to read return"""
        bRequest = 8         # or 8 or 9
        for i in range(255):
            try:
                ret = self.dev.ctrl_transfer(0x80, bRequest, i, 0, 4)
                print i,
                print ret
            except:
                # failed to get data for this request
                pass

    def beginCom(self):
        """Send weird values needed to control kinect motor"""
        # (bmRequestType, bmRequestType, bmRequest, wValue, wIndex, nBytes)
        ret = self.dev.ctrl_transfer(0xC0, 0x10, 0x0, 0x0, 1)
        print(hex(ret[0]))   # should return 0x22 but dont know why ?
        # ???
        return self.dev.ctrl_transfer(0x40, 0x6, 0x1, 0x0, [])
        time.sleep(1.5)

    def headUp(self):
        """Turn up the camera"""
        return self.dev.ctrl_transfer(0x40, 0x31, 0xfff0, 0x0, [])

    def headDown(self):
        """Turn down the camera"""
        return self.dev.ctrl_transfer(0x40, 0x31, 0xffd0, 0x0, [])

    def GetAccelData(self):
        """Return acceleration data"""
        # Get accel data
        ret = self.dev.ctrl_transfer(0xC0, 0x32, 0x0, 0x0, 10)
        print map(hex, ret)
        # bytes 0 & 1 are always zero
        x = (ret[2] << 8) | ret[3]
        x = (x + 2**15) % 2**16 - 2**15     # convert to signed 16b
        y = (ret[4] << 8) | ret[5]
        y = (y + 2**15) % 2**16 - 2**15     # convert to signed 16b
        z = (ret[6] << 8) | ret[7]
        z = (z + 2**15) % 2**16 - 2**15     # convert to signed 16b
        print(x, "\t", y, "\t", z)
        return ret



if __name__ == '__main__':
    Utils().debug('Kinect motor control !')
    args = Utils().parseArgs("Whatever", True)
    Utils().loadJsonConfig(args.file)
    kMotor = KinectMotor(0x045e, 0x02B0)
    kMotor.configure()
    ret = kMotor.beginCom()
    ret = kMotor.headUp()
    print ret
    time.sleep(2)
    #ret = kMotor.headDown()
    #time.sleep(2)
    #Utils().fatal('it worked !')
