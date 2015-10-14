/*********************************************************************
*	フェードイン・アウト フィルタプラグイン     for AviUtl
*	                              ver 0.01
*
*	                            作成 : 2002/ 6/15～ 更新 : 6/16
*
*	 6/16	とりあえず完成。
*********************************************************************/
#include <windows.h>
#include "filter.h"

//----------------------------
//	フィルタ構造体
//----------------------------

#define TRACK_N 2	// トラックバーの数
TCHAR *track_name[]   = { "   ＩＮ", "  ＯＵＴ" };	// トラックバーの名前
int   track_default[] = { 0, 0 };	// トラックバーの初期値

FILTER_DLL filter = {
	FILTER_FLAG_PRIORITY_LOWEST | FILTER_FLAG_EX_INFORMATION | FILTER_FLAG_AUDIO_FILTER,	//	フィルタのフラグ
	NULL,NULL,				// 設定ウインドウのサイズ
	"フェードイン・アウト",	// フィルタの名前
	TRACK_N,              	// トラックバーの数
	track_name,     	// トラックバーの名前郡
	track_default,  	// トラックバーの初期値郡
	NULL,NULL,	// トラックバーの数値の下限上限
	NULL,NULL,NULL,	// チェックボックス
	func_proc,  	//	フィルタ処理関数
	NULL,NULL,  	// 開始時,終了時に呼ばれる関数
	NULL,       	// 設定が変更されたときに呼ばれる関数
	NULL,       	// 設定ウィンドウプロシージャ
	NULL,NULL,  	// システムで使用
	NULL,NULL,  	// 拡張データ領域,サイズ
	"Audio Fade I/O ver0.01 by.MakKi",	// フィルタ情報
	NULL,NULL,		// セーブ開始・終了時に呼ばれる
};

/*********************************************************************
*	DLL Export
*********************************************************************/

EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	フィルタ処理関数
*===================================================================*/

BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	int i,j;
	int start,end;
	int sample;
	double temp;

	// 選択開始・終了フレーム番号取得
	fp->exfunc->get_select_frame(fpip->editp,&start,&end);

	sample = fpip->audio_n * fpip->audio_ch;	// １フレームのサンプル数

	// フェードイン
	if(fpip->frame-start < fp->track[0]){
		for(i=0;i<sample;i+=fpip->audio_ch){
			for(j=0;j<fpip->audio_ch;j++){
				temp = ((fpip->frame-start)+(double)i/sample)/fp->track[0];
				if(temp>1) temp = 1;
				fpip->audiop[i+j] *= temp;
			}
		}
	}

	// フェードアウト
	if(end-fpip->frame < fp->track[1]){
		for(i=0;i<sample;i+=fpip->audio_ch){
			for(j=0;j<fpip->audio_ch;j++){
				temp = 1-((fpip->frame+fp->track[1]-end-1)+(double)i/sample)/fp->track[1];
				if(temp<0) temp = 0;
				fpip->audiop[i+j] *= temp;
			}
		}
	}

	return TRUE;
}

