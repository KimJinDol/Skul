#pragma once

#include "../Ref.h"

/*
-Shader��?
���̴��� ȭ�鿡 ����� �ȼ��� ��ġ�� ������ ����س��� �Լ��̴�.
���̴��� �⺻ c++�ڵ�ó�� �̸� �����ϵǾ� �������Ͽ� ���Ե� ä�� �۾��Ǵ� ����� �ƴ϶�,
�̹��� ����ó�� �ʿ��� �� �ε��ؼ� ���� ����̴�.


Vertex Shader : ������ ���� ��ȯ�ܰ踦 ó���ϴ� Shader
Pixel Shader : �ȼ��� �÷��� ���ϴ� Shader
HullShader : �׼����̼� �� �ܰ迡�� ������ ��� ��������, �󸶳� ���������� �����ϴ� Shader
�׼����̼� : ������ ����ȭ���Ѽ� �������� �޽ø� ����� �� �ְ� ���ִ� ���.

hlsl (high-level-shader language) : DirectxX3D ���� ��� ���̴� ���. ���̴��� © �� �� hlsl�������̽��� �����Ͽ� �����!
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

    // ���ؽ��� ��ġ���� �̿��� �븻��, �÷� �ؽ�ó ��ǥ �� �پ��� ������ ���� �� �ִ�. ���ؽ��� ���� ����ü�� ǥ���Ǳ� ������ 
    // Input Layout�� ����Ͽ� GPU���ٰ� ���ؽ� ����ü�� ���� ���п� ���� ������ �־�� �Ѵ�. ���ؽ� ������ �Ӽ��� �Ʒ��� D3D11_INPUT_ELEMENT_DESC ����ü�� ������ �� �ִ�.
    ID3D11InputLayout* m_InputLayout;

    // D3D11_INPUT_ELEMENT_DESC : ������ �Ѱ��� ��Ҹ� �����Ҷ� ����ϴ� ����ü�̴�.
    // ���� ��� ��ġ, ������ ���� ������ �� ����ü 2���� �̿��ؼ� ���� �ϳ��� ǥ���Ѵ�.
    // �������� D3D11_INPUT_ELEMENT_DESC ����ü�� �迭�� �����ؼ� ���ؽ� ��ü�� ���� Input Layout������Ʈ�� �����.
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

    // �ø�ƽ�� �߰��ϴ� �Լ�. 
    // DXGI_FORMAT  : �ش� �ø�ƽ�� ����� �ڷ� ������ ���Ѵ�.
    // InputSlot    : �� ������ �ڷᰡ ���޵� ���� ���� ������ ����.
    void AddInputDesc(const char* SemanticName, unsigned int SemanticIndex, DXGI_FORMAT Fmt,
        unsigned int InputSlot, unsigned int Size, D3D11_INPUT_CLASSIFICATION InputSlotClass,
        unsigned int InstanceDataStepRate);
    virtual bool CreateInputLayout();
    virtual void SetShader(); // ���̴��� ����Ϸ��� ����ϰ��� �ϴ� ���̴��� �����ϸ� �Ǿ��־�߸� �Ѵ�. SetShader()�Լ����� �ش� ����� ����.
};

