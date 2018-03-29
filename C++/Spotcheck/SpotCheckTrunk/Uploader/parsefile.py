__author__ = 'chaudk'

import os, base64

_resources_path_ = '.\\SPOT\\'

class ParseFile(object):

    def __init__(self):
        self.SpotCheckROMfs ={}
        self.SpotCheckROMfs[5] = None
        self.SpotCheckROMfs[6] = None
        self.SpotCheckROMfs[7] = None
        self.SpotCheckROMfs[8] = None

    def parseResFiles(self, path, romfs_num, device_model):
        spc_rflag = False
        spc_romfs = "ROMfs" + str(romfs_num)

        if device_model == 'M5':
             for filename in os.listdir(path):
                if filename == ('SPOTCheckM5ROMfs'+str(romfs_num)+'.rsa'):
                    spc_rflag = True
                    self.SpotCheckROMfs[romfs_num] = path+ filename
                    break
        else :
            for filename in os.listdir(path):
                if filename == ('SPOTCheckM3ROMfs'+str(romfs_num)):
                    spc_rflag = True
                    self.SpotCheckROMfs[romfs_num] = path+ filename
        return spc_rflag

    def getPackageName(self, filename):
        f = open(filename, 'rb')
        fileSize = os.path.getsize(filename)
        f.seek(fileSize - 248 + 1)
        data = f.read(248)
        data=data.decode('latin-1')
        pckg_info = ''
        i = 0
        pckg_info+=data[i:data.find('\x00', i+1)]
        i += len(data[i:data.find('\x00', i+1)])
        i += len(data[i:data.find('\x00', i+1)])
        i += len(data[i:data.find('\x00', i+1)])
        i += len(data[i:data.find('\x00', i+1)])
        i += len(data[i:data.find('\x00', i+1)])
        i += 1
        pckg_version=data[i:data.find('\x00', i+1)]
        loc = pckg_version.find('_')
        if loc > 0:
            pckg_info += pckg_version[loc:pckg_version.__len__()]

        f.close()
        return pckg_info

    def calcCert(self, ppn, pkgName, fileID, certBuf, index = -1):
        if fileID[:6] != 'ROMfs5' and fileID[:6] != 'ROMfs6' and fileID[:6] != 'ROMfs7' and fileID[:6] != 'ROMfs8':
            return None

        certFile = None

        if index >= 0:
            certFile = pkgName + '.lic' + str(index)
        else:
            certFile = pkgName + '.lic'

        item_list = None
        index += 1
        filePtr = 0

        certBuf=certBuf.decode('latin-1')
        certItemList = certBuf.split('\x00')    #Split certificate items
        try:
            with open(certFile,'rb') as toFile:
                fileSize = os.path.getsize(certFile)
                initPPN = toFile.read(8)
                initPPN=initPPN.decode('latin-1')
                filePtr += 9
                fileSize -= filePtr
                ppnDiff = int(ppn) - int(initPPN)
                pos = ppnDiff*4
                if fileSize <= pos:
                    return None
                else:
                    toFile.seek(filePtr + pos)
                    MAC = toFile.read(4)
                    MAC = MAC.decode('latin-1')
                    certItemList[11] = MAC
                    certBuf = '\x00'
                    for x in range(1,12):
                        certBuf += certItemList[x]
                        certBuf += '\x00'
                    len = certBuf.__len__()
                    return certBuf
        except:
            return certBuf