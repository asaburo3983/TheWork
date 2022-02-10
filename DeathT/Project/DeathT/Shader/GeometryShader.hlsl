// �W�I���g�� �V�F�[�_�̓��̓f�[�^��`
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)
};
struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;

	// �ǉ�
	float2 texture_pos : TEXTURE0;

	float3 PosSM: POSITION_SM; // ���_���W(�V���h�E�}�b�v�̓������W�n)
};
cbuffer ConstantBuffer
{
	float4x4	World;				// ���[���h�ϊ��s��
	float4x4	View;				// �r���[�ϊ��s��
	float4x4	Projection;			// �����ˉe�ϊ��s��
	float4		CameraPos;			// �J�������W
	float4		LightVector;		// ���C�g����
	float4		LightColor;			// ���C�g�J���[
	float4		MaterialAmbient;	// �A���r�G���g
	float4		MaterialDiffuse;	// �f�B�t���[�Y
	float4		MaterialSpecular;	// �X�y�L�����[

	matrix SMWorldViewProj;
	float4x4 boneMatrix[50];


	// ���[���h�~�r���[�~�����ϊ��s��(�V���h�E�}�b�v�p)
}
// �W�I���g�� �V�F�[�_�̊֐�
[maxvertexcount(3)]
void main(triangle GS_IN input[3],
	inout TriangleStream<PS_IN> TriStream) {
	PS_IN output;

	// �e���_�̌v�Z
	for (int i = 0; i < 3; ++i) {
		output.pos = input[i].pos;
		output.color = input[i].color;
		output.texture_pos = input[i].texture_pos;
		output.PosSM = input[i].PosSM;

		// �o��
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}