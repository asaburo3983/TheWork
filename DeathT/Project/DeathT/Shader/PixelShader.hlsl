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

	float4 multiplyColor;
	// ���[���h�~�r���[�~�����ϊ��s��(�V���h�E�}�b�v�p)
}
Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
Texture2D    TextureSM : register(t0[1]);

//Texture2D    TextureSM3 : register(t0[3]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM4 : register(t0[4]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM5 : register(t0[5]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM6 : register(t0[6]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM7 : register(t0[7]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM8 : register(t0[8]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM9 : register(t0[9]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
//Texture2D    TextureSM10 : register(t0[10]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�



SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�
SamplerState SamplerSM : register(s0[1]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�




float4 main(PS_IN input) : SV_Target
{
	// �e�N�X�`���J���[�̎擾
	float4 tex_color = Texture.Sample(Sampler, input.texture_pos);
	//if (tex_color.a <= 0.0)discard;//a�O�l��\������

	// �V���h�E�}�b�v
	float sm = TextureSM.Sample(SamplerSM, input.PosSM.xy);

	float sma = (input.PosSM.z < sm) ? 1.0 : 0.5;



	//�}�e���A���ƌ����Ή��i��Ԃ��ꂢ�j
	float ambient_factor = MaterialAmbient[3];
	float diffuse_factor = MaterialDiffuse[3];
	float specular_factor = MaterialSpecular[3];	// �X�y�L�����[

	float4 ambient_color = MaterialAmbient * ambient_factor;
	float4 diffuse_color = input.color * (LightColor * MaterialDiffuse) * diffuse_factor;
	//float4 specular_color = *specular_factor

	//�A���`�G�C���A�X�ɔ����F�̃o�C�A�X
	/*tex_color.r = pow(tex_color.r, 2.2);
	tex_color.g = pow(tex_color.g, 2.2);
	tex_color.b = pow(tex_color.b, 2.2);*/

	//return sma;
	tex_color *= multiplyColor;
	tex_color.x *= sma;
	tex_color.y *= sma;
	tex_color.z *= sma;

	//tex_color.a = 1.0f;
	return tex_color;

	//�}�e���A���ƌ����Ή��i��Ԃ��ꂢ�j�z�̎����Ƃ����o��
	//return (ambient_color + diffuse_color * tex_color)*sma;

	//�����Ή�(�Â��ĉ����̂�*2���Ă���) �A�j�����A�F����������Əo��@�����͂Ȃ�
	return input.color * tex_color * 2;

	//�e�N�X�`���J���[�̂�(�A�j����)
	return tex_color;

}
