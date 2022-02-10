#define _CRT_SECURE_NO_WARNINGS
#include "MaterialLinkerData.h"
namespace frauEngine {
	void LoadMaterialLinker(const char* _linkerPath, const char* _materialPath, const char* _texPath, ModelObject* _modelObj) {

		//マテリアルリンカーからマテリアル名のリストを取得
		MaterialLinkerData data;
		FILE* fp = fopen(_linkerPath, "rb");
		fread(&data, sizeof(data), 1, fp);
		fclose(fp);

		int _meshMax = _modelObj->GetMeshNum();

		for (int i = 0; i < _meshMax; i++) {
			//リンカーパスから取り出したマテリアル名とマテリアルパスを合体して相対パスを作成
			MaterialData materialData;
			char tmp2[256] = "";
			sprintf(tmp2, _materialPath);
			strcat(tmp2, data.materialPath[i]);

			FILE* fp2 = fopen(tmp2, "rb");
			fread(&materialData, sizeof(MaterialData), 1, fp2);
			fclose(fp2);

			//アナザーテクスチャが有効ならテクスチャをロードする
			for (int h = 0; h < (int)frauEngine::ShaderTextureSlot::MODEL_ANOTHER; h++) {
				if (materialData.anotherEnable[h]) {
					char tmp[256] = "";
					sprintf(tmp, _texPath);
					strcat(tmp, materialData.anotherTexturePath[h]);
					_modelObj->SetAnotherTexture(i, tmp, h);
				}
			}

			//PBRのデータをモデルにロードする
			_modelObj->SetPBR(
				i,
				materialData.subsurface,
				materialData.metallic,
				materialData.specular,
				materialData.specularTint,
				materialData.roufhness,
				materialData.anistropics,
				materialData.sheen,
				materialData.sheenTint,
				materialData.clearColor,
				materialData.clearcoatGloss,
				materialData.outline,
				materialData.outlineColor,
				NULL
			);
		}

	}
}