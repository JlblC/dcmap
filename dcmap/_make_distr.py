# -*- coding: cp1251 -*-
import os, sys, re
import distutils.file_util
import distutils.dep_util

execfile("googlecode_upload.py")

output  = "E:/dcmap_dev/distr/"

def Delete(file):
    try: os.remove(file)
    except OSError: I=0

def MakeUpdateFile(path,aver,ver,fn):
    tver = ver.replace('_','.')
    
    f = open(output+fn+".txt",'w')

    f.write('DCMap Web Update\n')
    f.write(aver+'\n')
    f.write(tver+'\n')
    f.write('http://dcmap.googlecode.com/files/'+path+ver+".upd"+'\n')
    f.close()

def Upload(file_path,summary):

    status, reason, url = upload(file_path, 'dcmap', 'yarryp', 'jD8BY5zY7Dj5',
                                 summary, '')
    
    if url:
        print 'The file was uploaded successfully.'
        print 'URL: %s' % url
    else:
        print 'An error occurred. Your file was not uploaded.'
        print 'Google Code upload server said: %s (%s)' % (reason, status)

 
def MakeUpdate(path,ver,tver):
    tver = ver.replace('_','.')
    
    f = open("dcmap_comment_temp.txt",'w')

    f.write('Setup=dcmap.exe\nSilent=2\nOverwrite=1\nTitle=DCMap\nLicense=DCMap\n{\n');
    f.write('Обновление до версии '+tver+'\n}')
    f.close()

    os.system('"C:/program files/winrar/winrar" S -y -ibck -zdcmap_comment_temp.txt '+output+path+ver+".rar")

    Delete("dcmap_comment_temp.txt")
    Delete(output+path+ver+".upd")

    file_path = output+path+ver+".upd";
    
    os.rename(output+path+ver+".exe",file_path)
    #Upload(file_path,tver+" update") 


def Compress(path,ver,tver,ext):
    archn = output+path+ver+'.'+ext
    Delete(archn)
    cmd = '"C:/program files/winrar/winrar" A -r -ep1 -ibck -m5	-s '+archn+' '+output+path+'/*.*'
    os.system(cmd)
    #Upload(archn,tver) 

def MakeInstall(path,ver,tver,type,ext,src):
    archn = output+path+ver+'.'+ext
    Delete(archn)
    cmd = '"C:/Program Files/NSIS/makensis" /DVERSION='+tver+' /DOUTPUT='+archn+' /DITYPE='+type+' '+output+path+'/'+src+'.nsi'
    os.system(cmd)
    Upload(archn,"DCMap "+tver+" "+type+" "+src) 
    

def CopyToGlobe(path,ver,tver):
    distutils.file_util.copy_file("dcmap.exe",output+path)
    distutils.file_util.copy_file("dcmapsys.dll",output+path)
    Compress(path,ver,tver,'zip')
    Compress(path,ver,tver,'rar')
    MakeUpdate(path,ver,tver)
 

def CopyTo(path,ver,tver,type):
    distutils.file_util.copy_file("dcmap.exe",output+path)
    distutils.file_util.copy_file("dcmap.dcm",output+path)
    distutils.file_util.copy_file("dcmap.lng",output+path)
    distutils.file_util.copy_file("dcmapsys.dll",output+path)
    distutils.file_util.copy_file("_history.rtf",output+path)
    distutils.file_util.copy_file("keywords.txt",output+path)
    distutils.file_util.copy_file("dcm.exe",output+path)


    #Compress(path,ver,tver,'zip')
    #Compress(path,ver,tver,'rar')
    #MakeUpdate(path,ver,tver)
    MakeInstall(path,ver,tver,type,'exe','install')
    MakeInstall(path,ver,tver,type,'upd','update')

f = open("dcmapbase.h")
line = f.read()
f.close()
aver = re.search(r"DCMAP_VERSION\s+\"([a.0-9]+)\"",line).group(1)
tver = re.search(r"\"v([.0-9]+)\s*([a-z]*)\"",line).group(1)
ver = tver.replace('.','_');
beta = re.search(r"DCMAP_BETA\s+([0-9]+)",line).group(1) == '1'

type = '';

if beta :
    type = 'beta'


CopyTo("dcmap",ver,tver,type)

#CopyToGlobe("dcmapglobe",ver,"dcmapglobe "+tver)

if not beta :
    MakeUpdateFile("dcmap",aver,ver,"current")

MakeUpdateFile("dcmap",aver,ver,"beta")
#MakeUpdateFile("dcmapglobe",aver,ver,"dcmapglobe")


             
