object WebModule: TWebModule
  OldCreateOrder = False
  Height = 232
  Width = 340
  object IdCookieManager: TIdCookieManager
    Left = 112
    Top = 16
  end
  object IdCompressorZLib1: TIdCompressorZLib
    Left = 208
    Top = 16
  end
  object IdHTTP: TIdHTTP
    OnStatus = IdHTTPStatus
    OnWork = IdHTTPWork
    OnWorkBegin = IdHTTPWorkBegin
    OnWorkEnd = IdHTTPWorkEnd
    AllowCookies = True
    HandleRedirects = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'DCMap'
    HTTPOptions = []
    CookieManager = IdCookieManager
    Compressor = IdCompressorZLib1
    Left = 32
    Top = 16
  end
  object IdAntiFreeze1: TIdAntiFreeze
    IdleTimeOut = 50
    OnlyWhenIdle = False
    Left = 104
    Top = 80
  end
end
