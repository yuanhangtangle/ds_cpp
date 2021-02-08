### LCS（最长相同子字符串）：

1. 减而治之

   这里其实相当于从刚开始的形式出发, 考虑最后一项的匹配结果, 从而将情况分解为一些子情况, 因而能够把结果求出来. 但是直接的递归会导致计算时间的空间的大幅消耗, 所以考虑从最基础的结果出发, 逆着思维过程解决这个问题.

2. 动态规划

   是上面减而治之想法的逆过程. 由于减而治之的过程包含了两个方向的子情况, 所以需要考虑先解决这两种情况, 记录其结果, 然后把最终结果求出来, 相当于进行了制表.

   <img src=".\pic\20209170829.png" style="zoom:25%;" />

这种两个变量的递归问题很多都可以按照递归的想法改写成动态规划的指标法. 比如底下的栈混洗Catalan数的计算, 咖啡豆存量的计算.

### 选取最大的两个元素

1. 求最大元素的问题可以理解为一个子集和它的补集的最大元素的最大者, 选取最大的两个元素的问题其实也是类似的. 这就导向了问题的递归解法.
2. 另一种更加直接的考量就是遍历序列, 然后反复检验.

### 唯一化

1. 有序唯一化
2. 无序唯一化
3. Bitmap的记录

### 二分查找

1. 比较两次，去除已经处理的元素，三分支，提前返回（version A）
2. 比较一次，已处理元素留在右边，二分支，提前返回（version B）
3. 比较一次，已处理元素不保留，二分支，最终返回（version C）

### 试探回溯法 (八皇后问题)

	1. 从第一行开始放置(0,0)
 	2. 将棋盘扩充为 9 x 9, 最后一个作为哨兵
 	3. 放置皇后的方法是, 从现有的列出发, 每次向下一列放置, 检查是否和已有的皇后冲突, 直到不冲突为止
 	4. 每处理一个皇后, 都将其放置到不冲突的位置. 如果是 9, 表示无处可放, 回到上一个结点
 	5. 以第一行的位置放置为结束标志, 如果在第一行放置了9, 那就表示放置结束了

### 滑动窗口问题

**从一个字符串``s[0:n)``中照出无重复字串的最大长度.**

1. 递归转动态规划

   For ``s``, its longest substring ``sub`` without repeating character can only be in the following two situations: ``sub``ends with ``s[n-1]``, or ``sub`` is a substring of ``s[0:n-1)``. 

2. 滑动窗口

   Imagine moving a window``[i,j]``through the string ``s``. Each time, we let ``j = j+1``, and check if ``s[j]`` is in ``s[i,j-1]``. if so, say ``s[k]==s[j]``we need to  record the length ``j-1``, and adjust ``i = k+1`` . Different ways to find ``c = s[j]`` in``s[i,j-1]``  leads to different methods:

   1. One simple way is to search for ``s[j]`` in ``s[i:j-1]``;

   2. Considering that ASCII contains only 128 characters, we can use a vector ``vector v[128,0]`` to record the position of the last seen  ``c`` when we add ``j`` with ``1``. This idea is a bit similar to ``bitmap``, but implemented with a ``vector`` for convenience.

      ```c++
      class Solution {
      public:
      //from leetcode
          int lengthOfLongestSubstring(string s) {
              vector<int> m(128, 0);
              int ans = 0;
              int i = 0;
              for (int j = 0; j < s.size(); j++) {
                  i = max(i, m[s[j]]);// adjust i
                  m[s[j]] = j + 1;// position + 1
                  ans = max(ans, j - i + 1);// record the length
              }
              return ans;
          }
      };
      ```

   ### 多下标检索

   **Given a ``vector<int> nums`` and a ``int target``, find the two indices``i, j`` of  ``nums`` such that the sum of the two elements of ``nums`` are equal to``target``**

   1. One simple idea is to traverse the vector twice and record the indices;

   2. Another idea is to use a Map``m`` record the index of each element in ``nums``by traversing the vector, and then we can traverse the vector again and check if ``target - nums[i]`` is among the keys of ``m``. This way works because the problem assumes that only one solution is available and thus ``nums`` contains no duplicate integer.

      ```c++
      class Solution {
      public:
          vector<int> twoSum(vector<int>& nums, int target) {
              map<int,int> a;//建立hash表存放数组元素
              vector<int> b(2,-1);//存放结果
              for(int i=0;i<nums.size();i++)
                  a.insert(map<int,int>::value_type(nums[i],i));
              for(int i=0;i<nums.size();i++)
              {
                  if(a.count(target-nums[i])>0&&(a[target-nums[i]]!=i))
                  //判断是否找到目标元素且目标元素不能是本身
                  {
                      b[0]=i;
                      b[1]=a[target-nums[i]];
                      break;
                  }
              }
              return b;
          };
      };
      ```

   

Here is another example:
<img src="D:\semester\ongoing\dsacpp\dsacpp_alg\299.png" style="zoom:70%;" />

I made some mistakes in the problem:

   - I did not read the statements carefully before I code, which caused lots of confusion that could had been avoided;
   - I was not thinking when I was coding;
   - I was not familiar with c++ functions associated with ``string`` class.

   Let's go back to the problem itself. It's very important to deal with the positions of the numbers and duplicated numbers. And keep it in mind that each number can only be used once. Here is a sample solution:

   ```c++
   class Solution {
   public:
       string getHint(string secret, string guess) {
           int arr_s[10] = {0,0,0,0,0,0,0,0,0,0}; //记录数字出现的次数
           int arr_g[10] = {0,0,0,0,0,0,0,0,0,0}; //同上
           int n = secret.size();
           int count_a = 0;
           int count_b = 0;
           char s, g;
           for(int i = 0; i < n; i++)
           {
               s = secret[i] - '0';
               g = guess[i] - '0';
               if(s == g) ++count_a;
               else{ //只有同一位置不同数字时才记录
                   arr_s[s]++;
                   arr_g[g]++;
               }
           }
           for(int i = 0; i < 10; i++)
           {
               count_b += (arr_g[i] < arr_s[i])? arr_g[i]:arr_s[i]; //小的一方
           }
           return to_string(count_a) + "A" + to_string(count_b) + "B";
       }
   };
   ```

Another  counting problem:

![](D:\semester\ongoing\dsacpp\dsacpp_alg\1002.png)

One thing worth noticing is that the ``string``'s contain only lower case letters, so we can use a two dimensional to store the occurrences of each letter in each ``string``. Then we need to just compute the smallest record for each letter. But if the ``string``'s can contain any legal character, then it may be better to use ``vector`` or structures alike:

```c++
class Solution {
public:
    vector<string> commonChars(vector<string>& A) {
        vector<int> total(128,0);//记录现有的字符最小次数
        vector<int> curr(128,0);//记录下一个字符串的字符次数
        vector<string> re;
        int n = A.size();
        int m;
        string s;
        for(int i = 0; i < n; i++)
        {
            s = A[i];
            m = s.size();
            if (i == 0) //第一个
            {
                for(int j = 0; j < m; j++) total[s[j]] ++;
                continue;
            }
            else
            {
                for(int j = 0; j < m; j++) curr[s[j]] ++;
                for(int j = 0; j < 128; j++)//比较两者，记录小的一方
                {
                    total[j] = total[j] < curr[j]? total[j]:curr[j];
                    curr[j] = 0; //clear
                }
            }
        }

        for(int i = 0; i < 128; i++)//构成vector
        {
            m = total[i];
            for(int j = 0; j < m; j++)
            {
                s = char(i);
                re.push_back(s);
            }
        }
        return re;
    }
};
```

## 路径查找

![](D:\semester\ongoing\dsacpp\dsacpp_alg\1462.png)

```c++
class Solution {
public:
    vector<bool> checkIfPrerequisite(int n, vector<vector<int>>& prerequisites, vector<vector<int>>& queries) {
        vector<vector<bool>> matrix(n, vector<bool>(n, false)); //路劲矩阵
        vector<bool> result;//(queries.size(), false);
        vector<bool> states(n, false); //记录结点``i``是否已经DFS
        for(int i = 0; i < prerequisites.size(); i++) //记录已有的路径,相当于邻接矩阵
            matrix[prerequisites[i][0]][prerequisites[i][1]] = true;
        
        for(int i = 0; i < queries.size(); i++)
        {
            if(!states[queries[i][0]])//如果没有搜索过
                dfs(matrix, states, queries[i][0]);//就进行一次dfs
            result.push_back(matrix[queries[i][0]] [queries[i][1]]);
            //此时queries[i][0]]能够到达的结点都已经在matrix中标明, 故只要检索matrix即可
        }
        return result;
    }

   void dfs(vector<vector<bool>>& matrix, vector<bool>& states, int start)
   {
       for(int i = 0; i < matrix[start].size(); i++)
       {
           if(matrix[start][i])//对所有和start结点相邻的结点
           {
               if(!states[i])//如果没有搜索过
                    dfs(matrix, states, i);//就进行一次递归的搜索
                for(int j = 0; j < matrix[i].size(); j++)//如果i能达到j
                    if(matrix[i][j]) matrix[start][j] = true;//那么start也能达到j,而且start经由i能
           }                                                 //够达到的所有结点都记录到了
       }
       states[start] = true;//已经搜索过的结点做记录, 不再搜索
   }
};
```



## 无重复枚举方法

<img src="D:\semester\ongoing\dsacpp\dsacpp_alg\18.png" style="zoom:100%;"/>

这个题目就是个循环枚举题，问题在于如何保证枚举不出现重复，一种粗暴的方法是先枚举再去重；另一种相对好一点的方法则是先排序，在进行循环枚举的时候避免同一层循环取到相同值

 ```c++
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> quadruplets;
        if (nums.size() < 4) {
            return quadruplets;
        }
        sort(nums.begin(), nums.end());// 排序
        
        int length = nums.size();
        for (int i = 0; i < length - 3; i++) {//第一层循环
            if (i > 0 && nums[i] == nums[i - 1]) {
                continue;//重复枚举
            }
            if (nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target) {
                break;//剪枝
            }
            if (nums[i] + nums[length - 3] + nums[length - 2] + nums[length - 1] < target) {
                continue;//剪枝
            }
            
            for (int j = i + 1; j < length - 2; j++) {//第二层循环
                if (j > i + 1 && nums[j] == nums[j - 1]) {
                    continue;
                }
                if (nums[i] + nums[j] + nums[j + 1] + nums[j + 2] > target) {
                    break;
                }
                if (nums[i] + nums[j] + nums[length - 2] + nums[length - 1] < target) {
                    continue;
                }//多个剪枝
                
                //双指针
                int left = j + 1, right = length - 1;
                while (left < right) {
                    int sum = nums[i] + nums[j] + nums[left] + nums[right];
                    if (sum == target) {
                        quadruplets.push_back({nums[i], nums[j], nums[left], nums[right]});
                        while (left < right && nums[left] == nums[left + 1]) {
                            left++;
                        }//跳过重复值
                        left++;
                        while (left < right && nums[right] == nums[right - 1]) {
                            right--;
                        }//跳过重复值
                        right--;
                    } else if (sum < target) {//指针包夹
                        left++;
                    } else {
                        right--;
                    }
                }
            }
        }
        return quadruplets;
    }
};
 ```



## 红白蓝问题

![](D:\semester\ongoing\dsacpp\dsacpp_alg\75.png)

- 计数排序：直接记录0，1，2的数量，重写；

- 三指针方法：

  ```c++
  class Solution {
  public:
      void sortColors(vector<int>& nums) {
          for(int k = 0, i = 0, j = nums.size() - 1; i <= k && k <= j;)
          {
              if(nums[k] == 2)
              {
                  swap(nums[k], nums[j]);//把 2 换到 j 位置
                  for(;-1 < j && nums[j] == 2; j--); 
              }
              else if(nums[k]==0)
              {
                  swap(nums[k], nums[i]);
                  for(; i < n && nums[i]==0;i++);//把 0 换到 i 位置
                  if(k < i) k = i;//保持 k 的位置
              }
              else{
                  k++; //如果是 1, 直接前进
              }
          }
      }
  };
  ```

  

## 串的分拆

![](D:\semester\ongoing\dsacpp\dsacpp_alg\139.png)

只好动态规划制表了，下面是官方答案：

```c++
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        auto wordDictSet = unordered_set <string> ();
        for (auto word: wordDict) {
            wordDictSet.insert(word);
        }

        auto dp = vector <bool> (s.size() + 1);
        dp[0] = true;
        for (int i = 1; i <= s.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (dp[j] && wordDictSet.find(s.substr(j, i - j)) != wordDictSet.end()) {
                    dp[i] = true;
                    break;
                }
            }
        }

        return dp[s.size()];
    }
};
```

