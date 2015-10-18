/*********************************************************************
*	フェードイン・アウト フィルタプラグイン     for AviUtl
*	                              ver 0.02
*
*	                            作成 : 2002/ 6/15～ 更新 : 7/ 6
*
*	 6/16	とりあえず完成。(0.01)
*	 7/ 6	虫退治、無音挿入機能を追加。(0.02)
*
*********************************************************************/
#include <windows.h>
#include "filter.h"

//----------------------------
//	フィルタ構造体
//----------------------------

#define TRACK_N 4	// トラックバーの数
TCHAR *track_name[]   = { "fade in", "fade out", "silent st", "silent ed" };	// トラックバーの名前
int   track_default[] = { 0, 0, 0, 0 };	// トラックバーの初期値

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
	"Audio Fade I/O ver0.02 by MakKi",	// フィルタ情報
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
	int fade_st, fade_ed;
	int sample;
	double temp;

	// 選択開始・終了フレーム番号取得
	fp->exfunc->get_select_frame(fpip->editp,&start,&end);

	if(fpip->frame<start || end<fpip->frame) return FALSE;

	sample = fpip->audio_n * fpip->audio_ch;	// １フレームのサンプル数

	// フェード開始・終了フレーム番号
	fade_st = start + fp->track[2];
	fade_ed = end   - fp->track[3];

	if(fpip->frame<fade_st || fade_ed<fpip->frame){
		for(i=0;i<sample;i++)  fpip->audiop[i] = 0;
	}

	// フェードイン
	else if(fpip->frame-fade_st < fp->track[0]){
		for(i=0;i<sample;i+=fpip->audio_ch){
			for(j=0;j<fpip->audio_ch;j++){
				temp = ((fpip->frame-fade_st)+(double)i/sample)/fp->track[0];
				if(temp>1) temp = 1;
				if(temp<0) temp = 0;
				fpip->audiop[i+j] *= temp;
			}
		}
	}

	// フェードアウト
	else if(fade_ed-fpip->frame < fp->track[1]){
		for(i=0;i<sample;i+=fpip->audio_ch){
			for(j=0;j<fpip->audio_ch;j++){
				temp = 1-((fpip->frame+fp->track[1]-fade_ed-1)+(double)i/sample)/fp->track[1];
				if(temp<0) temp = 0;
				if(temp>1) temp = 1;
				fpip->audiop[i+j] *= temp;
			}
		}
	}

	return TRUE;
}

