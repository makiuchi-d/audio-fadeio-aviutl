/*********************************************************************
*	�t�F�[�h�C���E�A�E�g �t�B���^�v���O�C��     for AviUtl
*	                              ver 0.02
*
*	                            �쐬 : 2002/ 6/15�` �X�V : 7/ 6
*
*	 6/16	�Ƃ肠���������B(0.01)
*	 7/ 6	���ގ��A�����}���@�\��ǉ��B(0.02)
*
*********************************************************************/
#include <windows.h>
#include "filter.h"

//----------------------------
//	�t�B���^�\����
//----------------------------

#define TRACK_N 4	// �g���b�N�o�[�̐�
TCHAR *track_name[]   = { "fade in", "fade out", "silent st", "silent ed" };	// �g���b�N�o�[�̖��O
int   track_default[] = { 0, 0, 0, 0 };	// �g���b�N�o�[�̏����l

FILTER_DLL filter = {
	FILTER_FLAG_PRIORITY_LOWEST | FILTER_FLAG_EX_INFORMATION | FILTER_FLAG_AUDIO_FILTER,	//	�t�B���^�̃t���O
	NULL,NULL,				// �ݒ�E�C���h�E�̃T�C�Y
	"�t�F�[�h�C���E�A�E�g",	// �t�B���^�̖��O
	TRACK_N,              	// �g���b�N�o�[�̐�
	track_name,     	// �g���b�N�o�[�̖��O�S
	track_default,  	// �g���b�N�o�[�̏����l�S
	NULL,NULL,	// �g���b�N�o�[�̐��l�̉������
	NULL,NULL,NULL,	// �`�F�b�N�{�b�N�X
	func_proc,  	//	�t�B���^�����֐�
	NULL,NULL,  	// �J�n��,�I�����ɌĂ΂��֐�
	NULL,       	// �ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐�
	NULL,       	// �ݒ�E�B���h�E�v���V�[�W��
	NULL,NULL,  	// �V�X�e���Ŏg�p
	NULL,NULL,  	// �g���f�[�^�̈�,�T�C�Y
	"Audio Fade I/O ver0.02 by MakKi",	// �t�B���^���
	NULL,NULL,		// �Z�[�u�J�n�E�I�����ɌĂ΂��
};

/*********************************************************************
*	DLL Export
*********************************************************************/

EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*====================================================================
*	�t�B���^�����֐�
*===================================================================*/

BOOL func_proc(FILTER *fp,FILTER_PROC_INFO *fpip)
{
	int i,j;
	int start,end;
	int fade_st, fade_ed;
	int sample;
	double temp;

	// �I���J�n�E�I���t���[���ԍ��擾
	fp->exfunc->get_select_frame(fpip->editp,&start,&end);

	if(fpip->frame<start || end<fpip->frame) return FALSE;

	sample = fpip->audio_n * fpip->audio_ch;	// �P�t���[���̃T���v����

	// �t�F�[�h�J�n�E�I���t���[���ԍ�
	fade_st = start + fp->track[2];
	fade_ed = end   - fp->track[3];

	if(fpip->frame<fade_st || fade_ed<fpip->frame){
		for(i=0;i<sample;i++)  fpip->audiop[i] = 0;
	}

	// �t�F�[�h�C��
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

	// �t�F�[�h�A�E�g
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

