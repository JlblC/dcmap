object WebGetModule: TWebGetModule
  OldCreateOrder = False
  Height = 228
  Width = 331
  object IdHTTP: TIdHTTP
    AllowCookies = True
    HandleRedirects = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    HTTPOptions = [hoForceEncodeParams]
    CookieManager = IdCookieManager1
    Compressor = IdCompressorZLibEx1
    Left = 40
    Top = 24
  end
  object IdCompressorZLibEx1: TIdCompressorZLibEx
    Left = 128
    Top = 24
  end
  object IdCookieManager1: TIdCookieManager
    Left = 240
    Top = 24
  end
end
