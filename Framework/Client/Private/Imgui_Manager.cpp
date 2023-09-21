#include "pch.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "Imgui_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CImgui_Manager)

CImgui_Manager::CImgui_Manager()
{
}

HRESULT CImgui_Manager::Ready_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

    ImGuiStyles();

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CImgui_Manager::Tick(_float fTimeDelta)
{
    m_bFrameReady = true;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    _bool bIsOpen = false;

    ImGui::Begin("Tool Box");

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        // "File" �޴��� ���� �޴� ������ �߰�
        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {
            // "Open" �޴� �������� Ŭ���� �� ������ �۾�
        }

        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
            // "Save" �޴� �������� Ŭ���� �� ������ �۾�
        }

        ImGui::EndMenu(); // "File" �޴� ����
    }

    if (ImGui::BeginMenu("Edit"))
    {
        // "Edit" �޴��� ���� �޴� ������ �߰�
        // �߰� �۾� ����

        ImGui::EndMenu(); // "Edit" �޴� ����
    }
    ImGui::EndMainMenuBar();


    if (ImGui::CollapsingHeader("Level"))
    {

#pragma region LEVEL1
        ImGui::BeginTabBar("");
        if (ImGui::BeginTabItem("Level 1"))
        {
            // ���� 1 �� ������ �̰��� �߰�
            ImGui::Indent(10);
            if (ImGui::CollapsingHeader("Terrain"))
            {
                // ���� 1 �ͷ��� ����
                int value = 0;
                ImGui::SeparatorText("Vertical");
                ImGui::SetNextItemWidth(50);
                ImGui::InputInt("##Vertical Input", &value, 0, 1000);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderInt("##Vertical Slider", &value, 0, 1000);

                int Hvalue = 0;
                ImGui::SeparatorText("Horizon");
                ImGui::SetNextItemWidth(50);
                ImGui::InputInt("##Horizon Input", &Hvalue, 0, 1000);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderInt("##Horizon Slider", &Hvalue, 0, 1000);

                ImGui::Spacing();

                if (ImGui::Button("Create Terrain"))
                {

                }
            }

            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Object"))
            {
                // ���� 1 ������Ʈ ����
                ImGui::BeginTabBar("ObjectTabs");

                if (ImGui::BeginTabItem("Monster"))
                {
                    // ���� 1 ���� ����
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Prop"))
                {
                    // ���� 1 ���� ����
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::EndTabItem();
        }
#pragma endregion

#pragma region LEVEL2
        if (ImGui::BeginTabItem("Level 2"))
        {
            // ���� 2 �� ������ �̰��� �߰�
            ImGui::Indent(10);
            if (ImGui::CollapsingHeader("Terrain"))
            {
                // ���� 2 �ͷ��� ����
                int value = 0;
                ImGui::SeparatorText("Vertical");
                ImGui::SetNextItemWidth(50);
                ImGui::InputInt("##Vertical Input", &value, 0, 1000);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderInt("##Vertical Slider", &value, 0, 1000);

                int Hvalue = 0;
                ImGui::SeparatorText("Horizon");
                ImGui::SetNextItemWidth(50);
                ImGui::InputInt("##Horizon Input", &Hvalue, 0, 1000);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::SliderInt("##Horizon Slider", &Hvalue, 0, 1000);

                ImGui::Spacing();

                if (ImGui::Button("Create Terrain"))
                {

                }
            }

            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Object"))
            {
                // ���� 2 ������Ʈ ����
                ImGui::BeginTabBar("ObjectTabs");

                if (ImGui::BeginTabItem("Monster"))
                {
                    // ���� 2 ���� ����
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Prop"))
                {
                    // ���� 2 ���� ����
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::EndTabItem();
        }
#pragma endregion



        // �ٸ� ���� �ǵ� �߰� ����

        ImGui::EndTabBar();
    }

    ImGui::End();

    

	return S_OK;
}

HRESULT CImgui_Manager::LateTick(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CImgui_Manager::Render()
{
    if (m_bFrameReady)
    {
        ImGui::Render();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
	return S_OK;
}

void CImgui_Manager::ImGuiStyles()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.FrameBorderSize = 0.f;
    style.ChildBorderSize = 2.f;
    style.TabBorderSize = 0.f;
    style.PopupBorderSize = 0.f;

    style.WindowPadding = ImVec2(5, 5);
    style.WindowRounding = 2.f;
    style.FramePadding = ImVec2(5, 5);
    style.FrameRounding = 2.f;
    style.ItemSpacing = ImVec2(5, 5);
    style.ItemInnerSpacing = ImVec2(5, 5);

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.1f, 0.5f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.1f, 0.3f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.15f, 0.2f, 1.f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.225f, 0.4f, 1.f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.05f, 0.075f, 0.1f, 1.f);

    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.f, 0.f, 0.f, 1.f);

    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.f, 0.f, 0.1f, 0.2f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5f, 0.6f, 0.9f, 1.f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.f, 1.f / 255.f * 150.f, 1.f, 1.f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.f, 1.f / 255.f * 130.f, 1.f, 1.f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 1.f / 255.f * 130.f, 1.f, 0.8f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.15f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.225f, 0.30f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.05f, 0.075f, 0.10f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(.8f, .8f, .8f, 1.f);

}

_bool CImgui_Manager::Is_MouseClickedGUI()
{
    ImGuiContext* Context = ImGui::GetCurrentContext();
    if (!Context)
        return false;

    for (int i = 0; i < Context->Windows.Size; i++)
    {
        ImGuiWindow* window = Context->Windows[i];
        if (window->Flags & ImGuiWindowFlags_NoMouseInputs)
            continue;

        if (window->Rect().Contains(Context->IO.MousePos))
            return true;
    }

    return false;
}

void CImgui_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
