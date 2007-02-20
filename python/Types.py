from Mixins import _SimpleParameterTypeBase, _ParameterTypeBase, _Parameterizable, _ConfigureComponent, _Labelable, _TypedParameterizable, _Unlabelable

import codecs
_string_escape_encoder = codecs.getencoder('string_escape')

class int32(_SimpleParameterTypeBase):
    @staticmethod
    def _isValid(value):
        return isinstance(value,int)
    @staticmethod
    def _valueFromString(value):
        if len(value) >1 and '0x' == value[:2]:
            return int32(int(value,16))
        return int32(int(value))


class uint32(_SimpleParameterTypeBase):
    @staticmethod
    def _isValid(value):
        return ((isinstance(value,int) and value >= 0) or
                (isinstance(value,long) and value >= 0) and value <= 0xFFFFFFFF)
    @staticmethod
    def _valueFromString(value):
        if len(value) >1 and '0x' == value[:2]:
            return uint32(long(value,16))
        return uint32(long(value))


class int64(_SimpleParameterTypeBase):
    @staticmethod
    def _isValid(value):
        return isinstance(value,int) or (
            isinstance(value,long) and
            (-0x7FFFFFFFFFFFFFFF < value <= 0x7FFFFFFFFFFFFFFF) )
    @staticmethod
    def _valueFromString(value):
        if len(value) >1 and '0x' == value[:2]:
            return uint32(long(value,16))
        return int64(long(value))


class uint64(_SimpleParameterTypeBase):
    @staticmethod
    def _isValid(value):
        return ((isinstance(value,int) and value >= 0) or
                (ininstance(value,long) and value >= 0) and value <= 0xFFFFFFFFFFFFFFFF)
    @staticmethod
    def _valueFromString(value):
        if len(value) >1 and '0x' == value[:2]:
            return uint32(long(value,16))
        return uint64(long(value))


class double(_SimpleParameterTypeBase):
    @staticmethod
    def _isValid(value):
        return True
    @staticmethod
    def _valueFromString(value):
        return double(float(value))


class bool(_SimpleParameterTypeBase):
    @staticmethod
    def _isValid(value):
        return (isinstance(value,type(False)) or isinstance(value(type(True))))
    @staticmethod
    def _valueFromString(value):
        if (value.lower() == 'true' or
            value.lower() == 't' or
            value.lower() == 'on' or
            value.lower() == 'yes' or
            value.lower() == '1'):
            return bool(True)
        if (value.lower() == 'false' or
            value.lower() == 'f' or
            value.lower() == 'off' or
            value.lower() == 'no' or
            value.lower() == '0' ):
            return bool(False)
        raise RuntimeError('can not make bool from string '+value)


class string(_SimpleParameterTypeBase):
    def __init__(self,value):
        super(string,self).__init__(value)
    @staticmethod
    def _isValid(value):
        return isinstance(value,type(''))
    def configValue(self,indent,deltaIndent):
        return self.formatValueForConfig(self.value())
    @staticmethod
    def formatValueForConfig(value):
        l = len(value)
        value,newL = _string_escape_encoder(value)
        if l != newL:
            #get rid of the hex encoding
            value=value.replace('\\x0','\\')
        if "'" in value:
            return '"'+value+'"'
        return "'"+value+"'"
    @staticmethod
    def _valueFromString(value):
        return string(value)


class InputTag(_ParameterTypeBase):
    def __init__(self,moduleLabel,productInstanceLabel=''):
        super(InputTag,self).__init__()
        self.__moduleLabel = moduleLabel
        self.__productInstance = productInstanceLabel
    def getModuleLabel(self):
        return self.__moduleLabel
    def setModuleLabel(self,label):
        self.__moduleLabel = label
    moduleLabel = property(getModuleLabel,setModuleLabel,"module label for the product")
    def getProductInstanceLabel(self):
        return self.__productInstance
    def setProductInstanceLabel(self,label):
        self.__productInstance = label
    productInstanceLabel = property(getProductInstanceLabel,setProductInstanceLabel,"product instance label for the product")
    def configValue(self,indent,deltaIndent):
        return self.__moduleLabel+':'+self.__productInstance
    @staticmethod
    def _isValid(value):
        return True
    def __cmp__(self,other):
        v = self.__moduleLabel <> other.__moduleLabel
        if not v:
            return self.__productInstance <> other.__productInstance
        return v
    @staticmethod
    def formatValueForConfig(value):
        return value.configValue('','')


class PSet(_ParameterTypeBase,_Parameterizable,_ConfigureComponent,_Labelable):
    def __init__(self,*arg,**args):
        #need to call the inits separately
        _ParameterTypeBase.__init__(self)
        _Parameterizable.__init__(self,*arg,**args)
    def value(self):
        return self
    @staticmethod
    def _isValid(value):
        return True
    def configValue(self,indent='',deltaIndent=''):
        config = '{ \n'
        for name in self.parameterNames_():
            param = getattr(self,name)
            config+=indent+deltaIndent+param.configTypeName()+' '+name+' = '+param.configValue(indent+deltaIndent,deltaIndent)+'\n'
        config += indent+'}\n'
        return config
    def copy(self):
        import copy
        return copy.copy(self)
    def _place(self,name,proc):
        proc._placePSet(name,self)
    def __str__(self):
        return object.__str__(self)


class _ValidatingListBase(list):
    """Base class for a list which enforces that its entries pass a 'validity' test"""
    def __init__(self,*arg,**args):        
        super(_ValidatingListBase,self).__init__(arg)
        if not self._isValid(iter(self)):
            raise TypeError("wrong types added to "+str(type(self)))
    def __setitem__(self,key,value):
        if isinstance(key,slice):
            if not self._isValid(value):
                raise TypeError("wrong type being inserted into this container")
        else:
            if not self._itemIsValid(value):
                raise TypeError("can not insert the type "+str(type(value))+" in this container")
        super(_ValidatingListBase,self).__setitem__(key,value)
    def _isValid(self,seq):
        for item in seq:
            if not self._itemIsValid(item):
                return False
        return True
    def append(self,x):
        if not self._itemIsValid(x):
            raise TypeError("wrong type being appended to this container")
        super(_ValidatingListBase,self).append(x)
    def extend(self,x):
        if not self._isValid(x):
            raise TypeError("wrong type being extended to this container")
        super(_ValidatingListBase,self).extend(x)
    def insert(self,i,x):
        if not self._itemIsValid(x):
            raise TypeError("wrong type being inserted to this container")
        super(_ValidatingListBase,self).insert(i,x)


class _ValidatingParameterListBase(_ValidatingListBase,_ParameterTypeBase):
    def __init__(self,*arg,**args):
        _ParameterTypeBase.__init__(self)
        super(_ValidatingParameterListBase,self).__init__(*arg,**args)
    def value(self):
        return list(self)
    def setValue(self,v):
        self[:] = []
        self.extend(v)
    def configValue(self,indent,deltaIndent):
        config = '{\n'
        first = True
        for value in iter(self):
            config +=indent+deltaIndent
            if not first:
                config+=', '
            config+=  self.configValueForItem(value,indent,deltaIndent)+'\n'
            first = False
        config += indent+'}\n'
        return config
    def configValueForItem(self,item,indent,deltaIndent):
        return str(item)
    @staticmethod
    def _itemsFromStrings(strings,converter):
        return (converter(x).value() for x in strings)


class vint32(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vint32,self).__init__(*arg,**args)
        
    @staticmethod
    def _itemIsValid(item):
        return int32._isValid(item)
    @staticmethod
    def _valueFromString(value):
        return vint32(*_ValidatingParameterListBase._itemsFromStrings(value,int32._valueFromString))


class vuint32(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vuint32,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return uint32._isValid(item)
    @staticmethod
    def _valueFromString(value):
        return vuint32(*_ValidatingParameterListBase._itemsFromStrings(value,uint32._valueFromString))

    
class vint64(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vint64,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return int64._isValid(item)
    @staticmethod
    def _valueFromString(value):
        return vint64(*_ValidatingParameterListBase._itemsFromStrings(value,int64._valueFromString))


class vuint64(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vuint64,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return uint64._isValid(item)
    @staticmethod
    def _valueFromString(value):
        return vuint64(*_ValidatingParameterListBase._itemsFromStrings(value,vuint64._valueFromString))

    
class vdouble(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vdouble,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return double._isValid(item)
    @staticmethod
    def _valueFromString(value):
        return vdouble(*_ValidatingParameterListBase._itemsFromStrings(value,double._valueFromString))


class vbool(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vbool,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return bool._isValid(item)
    @staticmethod
    def _valueFromString(value):
        return vbool(*_ValidatingParameterListBase._itemsFromStrings(value,bool._valueFromString))


class vstring(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(vstring,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return string._isValid(item)
    def configValueForItem(self,item,indent,deltaIndent):
        return string.formatValueForConfig(item)
    @staticmethod
    def _valueFromString(value):
        return vstring(*_ValidatingParameterListBase._itemsFromStrings(value,string._valueFromString))


class VInputTag(_ValidatingParameterListBase):
    def __init__(self,*arg,**args):
        super(VInputTag,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return InputTag._isValid(item)
    def configValueForItem(self,item,indent,deltaIndent):
        return InputTag.formatValueForConfig(item)
    @staticmethod
    def _valueFromString(value):
        return VInputTag(*_ValidatingParameterListBase._itemsFromStrings(value,InputTag._valueFromString))


class VPSet(_ValidatingParameterListBase,_ConfigureComponent,_Labelable):
    def __init__(self,*arg,**args):
        super(VPSet,self).__init__(*arg,**args)
    @staticmethod
    def _itemIsValid(item):
        return PSet._isValid(item)
    def configValueForItem(self,item,indent,deltaIndent):
        return PSet.configValue(item,indent+deltaIndent,deltaIndent)
    def copy(self):
        import copy
        return copy.copy(self)
    def _place(self,name,proc):
        proc._placeVPSet(name,self)