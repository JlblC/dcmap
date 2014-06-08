#pragma once

namespace gsys
{
	class blowfish
	{
	private:
		gsys_dword 	* PArray ;
		gsys_dword	(* SBoxes)[256];
	private:
		void 		Blowfish_encipher (gsys_dword *xl, gsys_dword *xr) ;
		void 		Blowfish_decipher (gsys_dword *xl, gsys_dword *xr) ;
		void 		Initialize (gsys_byte key[], int keybytes) ;
		gsys_dword	GetOutputLength (gsys_dword lInputLong) ;
		gsys_dword	Encode (gsys_byte * pInput, gsys_byte * pOutput, gsys_dword lSize) ;
		void		Decode (gsys_byte * pInput, gsys_byte * pOutput, gsys_dword lSize) ;
	public:
		blowfish() ;
		~blowfish() ;
		bool set_passphase(char const* passphase);
		bool encrypt(gsys_byte const* pData,int data_size,gsys_byte * pResultBuffer,int ResutSize);
		bool decrypt(gsys_byte const* pData,int data_size,gsys_byte * pResultBuffer,int ResutSize);
	};
}