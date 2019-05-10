
import example

dc =example.DMS_COMMON()
print "initial entryname is " + dc.fixed.entryName

dc.fixed.entryName ='in dc.fixed.entryName'
print "input = entryName:"  +dc.fixed.entryName
example.readentrybyname11(dc)
print "output = entryName:"  +dc.fixed.entryName
