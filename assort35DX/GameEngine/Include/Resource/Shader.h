#pragma once

#include "../Ref.h"

/*
-Shader란?
쉐이더는 화면에 출력할 픽셀의 위치와 색상을 계산해내는 함수이다.
쉐이더는 기본 c++코드처럼 미리 컴파일되어 실행파일에 포함된 채로 작업되는 방식이 아니라,
이미지 파일처럼 필요할 때 로딩해서 쓰는 방식이다.


Vertex Shader : 정점의 공간 변환단계를 처리하는 Shader
Pixel Shader : 픽셀의 컬러를 정하는 Shader
HullShader : 테셀레이션 전 단계에서 도형을 어떻게 분할할지, 얼마나 분할할지를 결정하는 Shader
테셀레이션 : 도형을 세분화시켜서 디테일한 메시를 출력할 수 있게 해주는 기능.

hlsl (high-level-shader language) : DirectxX3D 전용 고급 쉐이더 언어. 쉐이더를 짤 때 이 hlsl인터페이스를 응용하여 만든다!
*/

class CShader :
    public CRef
{
    friend class CShaderManager;

protected:
    CShader();
    virtual ~CShader() = 0;

protected:
    Shader_Type m_Type;

    // 버텍스는 위치정보 이외의 노말값, 컬러 텍스처 좌표 등 다양한 정보를 가질 수 있다. 버텍스는 보통 구조체로 표현되기 때문에 
    // Input Layout을 사용하여 GPU에다가 버텍스 구조체의 각각 성분에 대한 설명해 주어야 한다. 버텍스 각각의 속성은 아래의 D3D11_INPUT_ELEMENT_DESC 구조체로 설명할 수 있다.
    ID3D11InputLayout* m_InputLayout;

    // D3D11_INPUT_ELEMENT_DESC : 정점의 한가지 요소를 정의할때 사용하는 구조체이다.
    // 예를 들어 위치, 색상을 갖는 정점은 이 구조체 2개를 이용해서 정점 하나를 표현한다.
    // 여러개의 D3D11_INPUT_ELEMENT_DESC 구조체를 배열로 정의해서 버텍스 전체에 대한 Input Layout오브젝트를 만든다.
    std::vector<D3D11_INPUT_ELEMENT_DESC>   m_vecInputDesc;

    unsigned int        m_InputSize;

public:
    Shader_Type GetShaderType() const
    {
        return m_Type;
    }

    ID3D11InputLayout* GetInputLayout() const
    {
        return m_InputLayout;
    }

public:
    virtual bool Init();

    // 시멘틱을 추가하는 함수. 
    // DXGI_FORMAT  : 해당 시멘틱이 사용할 자료 형식을 말한다.
    // InputSlot    : 이 성분의 자료가 공급될 정점 버퍼 슬롯의 색인.
    void AddInputDesc(const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT Fmt,
        unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InputSlotClass,
        unsigned int InstanceDataStepRate);
    virtual bool CreateInputLayout();
    virtual void SetShader(); // 쉐이더를 사용하려면 사용하고자 하는 쉐이더가 컴파일링 되어있어야만 한다. SetShader()함수에서 해당 기능을 구현.
};

