#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include <stack>
#include <limits> 
#include <ios> 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <stdlib.h>
#include <windows.h>
#include <chrono>
#include <conio.h>
#include <time.h>
#include <queue>

using namespace std;
Renderer* rendererr;
VertexArray* vaa;
IndexBuffer* ibb;
int rows;
int cols;
int dRow[] = { 0, 1, 0, -1 };
int dCol[] = { -1, 0, 1, 0 };

void Draw(vector<vector<int>> &vis,Shader shader, vector<vector<glm::vec3>> translations, glm::mat4 proj, glm::mat4 view, GLFWwindow* window)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translations[i][j]);
            glm::mat4 mvp = proj * view * model;
            if (vis[i][j] == 2)
            {
                shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
            }
            else if (vis[i][j] == 1)
            {
                shader.SetUniform4f("u_Color", 1.0f, 0.63f, 1.0f, 1.0f);
            }
            else
            {
                shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

            }
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            rendererr->Draw(*vaa, *ibb, shader);
        }

    }
}

bool isValid(vector<vector<int>> &vis, int row, int col)
{
    if (row < 0 || col < 0 || row >= rows || col >= cols)
        return false;

    if (vis[row][col] == 1)
        return false;

    return true;
}

void DFS(int rowS, int colS, vector<vector<int>> &grid, vector<vector<int>> &vis, Shader shader, vector<vector<glm::vec3>> translations, glm::mat4 proj, glm::mat4 view, GLFWwindow* window)
{
   
    stack<pair<int, int> > st;
    st.push({ rowS, colS });
    while (!st.empty())
    {
        pair<int, int> curr = st.top();
        st.pop();
        int row = curr.first;
        int col = curr.second;

        if (!isValid(vis, row, col))
            continue;

        vis[row][col] = 1;
        cout << row << " " << col << "\n";

        if (grid[col][row] == 2)
        {
            vis[row][col] = 2;
            Draw(vis, shader, translations, proj, view, window);
            break;
        }
 
        Draw(vis, shader, translations, proj, view, window);
        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(400);
        for (int i = 0; i < 4; i++) {
            int adjx = row + dRow[i];
            int adjy = col + dCol[i];
            st.push({ adjx, adjy });
        }
        
    }
   
    
}
void BFS(int row, int col, vector<vector<int>> &grid, vector<vector<int>> &vis, Shader shader, vector<vector<glm::vec3>> translations, glm::mat4 proj, glm::mat4 view, GLFWwindow* window)
{
    queue<pair<int, int> > q;
    q.push({ row, col });

    while (!q.empty()) {

        pair<int, int> cell = q.front();
        int x = cell.first;
        int y = cell.second;

        q.pop();
        if (vis[x][y] != 1)
        {
            cout << x << " " << y << "\n";

            vis[x][y] = 1;
            if (grid[y][x] == 2)
            {
                vis[x][y] = 2;

                Draw(vis,shader,translations,proj,view,window);
                break;
            }
            Sleep(500);
            
        }
        Draw(vis, shader, translations, proj, view, window);
        glfwSwapBuffers(window);
        glfwPollEvents();

       
        // Go to the adjacent cells
        for (int i = 0; i < 4; i++) {

            int adjx = x + dRow[i];
            int adjy = y + dCol[i];

            if (isValid(vis, adjx, adjy)) {
                q.push({ adjx, adjy });
            }
        }
    }
}

int main(void)
{

    GLFWwindow* window;
    if (!glfwInit())
        return -1;
    window = glfwCreateWindow(960, 540, "Searching Visualizer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //////////////////////////////////////////////////////////////



    float positions[] = {
        0.0f,   0.0f,   0.0f,   0.0f,  //Bottom Left
        20.0f,  0.0f,   1.0f,   0.0f,  //Bottom Right
        20.0f,  20.0f,  1.0f,   1.0f,  //Top right
        0.0f,   20.0f,  0.0f,   1.0f   //Top Left 
    };
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb,layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,0));
    glm::vec4 vp(100.0f,100.0f,0.0f,1.0f);
    glm::vec4 result = proj * vp;

    Shader ss;
    ss.Bind();
    va.UnBind();
    vb.unbind();
    ib.unbind();

    Renderer renderer;
    
    float stepX = 0;
    float stepY = 0;
    float margin = 0;
    
    cout << "How many rows: ";
    cin >> rows;
    cout << "How many columns: ";
    cin >> cols;
    cout << "What is the row of the targeted location: ";
    int targetRow; cin >> targetRow;
    cout << "What is the column of the targeted location: ";
    int targetCol; cin >> targetCol;
    vector<vector<int>> map(rows, vector<int>(cols));
    vector<vector<int>> vis(rows, vector<int>(cols));
    map[targetRow-1][targetCol-1] = 2;
    cout << "Choose 1 for DFS and 2 for BFS: ";
    int option; cin >> option;

    vector<vector<glm::vec3>> translations(rows, vector<glm::vec3>(cols));

    /*int map[7][7] = {
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,2,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0}
    };*/

    /*int vis[7][7] = {
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0}
    };*/

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            translations[i][j].x += stepX;
            translations[i][j].y = 520 -  stepY;
            stepY += 25;
        }
        stepY = 0;
        stepX += 25;
    }

    Shader Wshader;
    Wshader.Bind();
    Wshader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
    Wshader.UnBind();
    vaa = &va;
    ibb = &ib;
    rendererr = &renderer;

    if (option == 1)
    {
        DFS(0, 0, map, vis, Wshader, translations, proj, view, window);
    }
    else if (option == 2)
    {
        BFS(0, 0, map, vis, Wshader, translations, proj, view, window);
    }
    else
    {
        return 0;
    }

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        Draw(vis, Wshader, translations, proj, view, window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}