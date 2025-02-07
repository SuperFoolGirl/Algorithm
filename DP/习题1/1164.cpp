/*
讲一下相加逻辑和取最值逻辑的区别：
1. 取最大值的逻辑通常用于求解最优解问题，比如求最大价值、最长路径、最大收益等。在这类问题中，我们需要在多个可选的决策中选择能使目标值达到最大的那个决策。每一个状态的最优解取决于它之前状态的最优解，并且在不同的转移路径中，我们只关注能带来最大结果的那条路径。
2. 相加的逻辑一般用于求解方案数问题，也就是计算满足特定条件的所有可能方案的总数。在这种情况下，每一种可能的决策都会产生一种新的方案，我们需要把所有可能决策产生的方案数累加起来，得到当前状态的总方案数。
3. 其他逻辑还有取最小值、相乘等，遇到再细说
*/

/*
讲一下递推和基准情况的选取
一、顺序递推适用情景
1. 问题具有明显的阶段性：当问题可以按照一定的顺序（如时间、步骤等）划分为多个阶段，并且每个阶段的状态可以由前一个阶段的状态推导出来时，适合使用顺序递推。例如，斐波那契数列的计算，每个数都可以由前两个数推导得出；还有背包问题中，依次考虑每个物品放入背包的情况。
2. 初始状态明确：如果问题的初始状态很容易确定，并且后续状态可以基于初始状态逐步推导，那么顺序递推会很方便。如上述菜品选择问题，初始状态是不选任何菜品，花费为 0 元，方案数为 1，从这个状态开始可以依次推导后续的状态。

二、逆序递推适用情景
1. 目标状态明确：当问题的目标状态比较清晰，并且可以从目标状态反向推导到初始状态时，逆序递推是一个不错的选择。例如，在一些路径规划问题中，已知终点状态，需要计算从起点到终点的路径数量或最优路径，此时可以从终点开始逆推到起点。
2. 问题的状态转移具有反向依赖关系：有些问题中，当前状态的计算依赖于后续状态的信息。比如在一些动态规划的博弈问题中，当前玩家的最优决策可能取决于后续玩家的可能决策，此时逆序递推可以更好地处理这种反向依赖关系。

硬要两个都写的话 虽然大多能写 但逻辑上不太顺畅。因此正逆序要根据题目选择
*/


/*
讲一下基准情况的确定
假设dp数组的范围是1-n
顺序递推的顺序是从1开始，可能会访问dp[0]，所以dp[0]或dp[1]是基准情况
逆序递推的顺序是从n开始，会访问dp[n+1]，所以dp[n+1]或dp[n]是基准情况
这里只是介绍了大致的方向，不过也是符合直觉的。递推起点肯定需要已知量，因此基准情况与起点同步
*/

// 1. 递归写法
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> dp(110, vector<int>(10000, 0)); // dp[i][j] = 选到第i个菜品后，花费了j元的方案数(i-n)
int n, m;
vector<int> menu(110, 0);

int dfs(int x, int money)
{
    if (dp[x][money])
        return dp[x][money];

    // 这里一开始想得太简单了啊！出口要处理基准情形啊，这是递归的基本结构
    if (x > n)
    {
        if (money == m)
            return 1; // 选到最后一个菜品，且正好花完了m元，方案自然只能是1，都写满了。这一点要好好理解
        return 0;
    }

    // 干掉sum 直接记忆化
    if (money + menu[x] <= m) // 可以选 也可以不选。但这里的逻辑是相加，不是取最大值。即当前状态为选或不选的方案数之和
        dp[x][money] = dfs(x + 1, money) + dfs(x + 1, money + menu[x]);
    else // 不能选
        dp[x][money] = dfs(x + 1, money);

    return dp[x][money];
}

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        cin >> menu[i];
    cout << dfs(1, 0); // 要体会这个这个初始状态！从第一个开始选，此时花了0元，意味着选择有很多很多
    return 0;
}



// 2. 顺序递推
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, m;
    vector<vector<int>> dp(110, vector<int>(10000, 0)); // dp[i][j] = 选到第i个菜品时，花费了j元的方案数(0-i) 注意与dfs是互补的
    cin >> n >> m;
    vector<int> menu(n + 1, 0);
    for (int i = 1; i <= n; i++)
        cin >> menu[i];

    // 基准情况：不选任何菜品，花费为 0 元，方案数为 1。这是唯一的基准情况
    dp[0][0] = 1;

    // 顺序遍历真的要体会那种递推的感觉，从前往后推，每一步都是基于前一步的结果
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            // 不选当前菜品的方案数
            dp[i][j] += dp[i - 1][j];
            // 这里要注意。每一步都是基于前一步的结果。如果已经花费的钱小于当前菜品的价格，那么当前菜品就不能选
            if (j >= menu[i])
            {
                // 加上选当前菜品的方案数
                dp[i][j] += dp[i - 1][j - menu[i]];
            }
        }
    }

    // 输出选完所有菜品且刚好花费 m 元的方案数
    // 递推是不断推向出口的过程，所以最后的结果是 dp[n][m]，与dfs相反。dfs由于是递归的，出口和入口是同一个
    cout << dp[n][m] << endl;
    return 0;
}



// 3. 逆序递推
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, m;
    cin >> n >> m;

    // 存储每个菜品的价格
    vector<int> menu(n + 1, 0);
    for (int i = 1; i <= n; i++)
    {
        cin >> menu[i];
    }

    // dp[i][j] 表示选到第 i 个菜品后，花费了 j 元的方案数
    vector<vector<int>> dp(n + 2, vector<int>(m + 1, 0));

    // 基准情况：选完所有菜品且花费为 m 元，方案数为 1
    // 最大的难点是这个基准情况
    dp[n + 1][m] = 1;

    // 逆序递推，从第 n 个菜品开始往前考虑
    for (int i = n; i >= 1; i--)
    {
        for (int j = 0; j <= m; j++)
        {
            // 不选当前菜品的方案数，继承自下一个状态
            dp[i][j] = dp[i + 1][j];

            // 如果选择当前菜品后总花费不超过预算 m
            if (j + menu[i] <= m)
            {
                // 加上选择当前菜品的方案数
                dp[i][j] += dp[i + 1][j + menu[i]];
            }
        }
    }

    // 输出从第 1 个菜品开始，花费为 0 元时，最终能刚好花完 m 元的方案数
    cout << dp[1][0] << endl;

    return 0;
}