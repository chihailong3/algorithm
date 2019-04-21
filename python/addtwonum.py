# -*- coding: utf-8 -


# Definition for singly-linked list.

# Definition  for a binary tree node.
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

class TrieNode(object):
    def __init__(self ):
        self.root =None
        self.is_word = False
        self.path_count = 0
        self.data = {}


class Trie:
    def __init__(self):
        """
        Initialize your data structure here.
        """
        self.root = TrieNode()

    def longestPrefix(self):
        node = self.root
        res = []
        while len(node.data) == 1 and not node.is_word:
            res.append(node.data.keys()[0])
            node = node.data[node.data.keys()[0]]
        return ''.join(res)
        pass


    def insert(self, word):
        """
        Inserts a word into the trie.
        :type word: str
        :rtype: void
        """
        node = self.root
        node.path_count+=1
        for chars in word:
            child = node.data.get(chars)
            if not child:
                node.data[chars] = TrieNode()
            node = node.data[chars]
            node.path_count +=1
        node.is_word = True

    def search(self, word):
        """
        Returns if the word is in the trie.
        :type word: str
        :rtype: bool
        """
        node = self.root
        for chars in word:
            node = node.data.get(chars)
            if not node:
                return False
        return node.is_word  # 判断单词是否是完整的存在在trie树中

    def startsWith(self, prefix):
        """
        Returns if there is any word in the trie that starts with the given prefix.
        :type prefix: str
        :rtype: bool
        """
        node = self.root
        for chars in prefix:
            node = node.data.get(chars)
            if not node:
                return False
        return True

    def get_start(self, prefix):
        """
          Returns words started with prefix
          :param prefix:
          :return: words (list)
        """

        def get_key(pre, pre_node):
            word_list = []
            if pre_node.is_word:
                word_list.append(pre)
            for x in pre_node.data.keys():
                word_list.extend(get_key(pre + str(x), pre_node.data.get(x)))
            return word_list

        words = []
        if not self.startsWith(prefix):
            return words
        if self.search(prefix):
            words.append(prefix)
            return words
        node = self.root
        for chars in prefix:
            node = node.data.get(chars)
        return get_key(prefix, node)

def testTrie():
    trie = Trie()
    trie.insert("")
    trie.insert("aa")


    r= trie.longestPrefix( )
    print(trie.search("key"))
    print(trie.search("somebody3"))
    print(trie.get_start('some'))



class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None

class ListNode(object):
    def __init__(self, x):
        self.val = x
        self.next = None
import random
import heapq
class Solution(object):

    def rotateRight(self, head, k):
        """
        :type head: ListNode
        :type k: int
        :rtype: ListNode
        """
        lenList = 0
        p,ptmp = head,head;
        while p :
            p=p.next
            lenList +=1
            if not p.next  :
                p.next  = ptmp
                break
        # m起始的位置
        m  = (lenList - k%lenList)%lenList
        p = head
        lastNode =head
        while m > 0 :
            lastNode = p
            p=p.next
            m-=1
        head = p # 把头指针放到相应起始位置
        lastNode.next = None
        while lenList>0 :
            if  not p.next:
                p.next = ptmp
            else:
                p  = p.next

            lenList -=1
        p.next = None
        return head






    def trap(self, height):
        """
        :type height: List[int]
        :rtype: int
        """
        def calculateRainFall(m,n,h):
            res = 0
            for i in range(m+1,n):
                res+= h-height[i]
            return res

        def top(s):
            return s[-1] if len(s)>0 else (999999,0)

        stack =[]

        rainFall = 0
        for idx, e in enumerate(height):
            if len(stack)==0 or top(stack)<e:
                stack.append((e,idx))
            else:
                while(top(stack)[0]<e):
                    hi,sn = stack.pop()
                    if len(stack)==0 and (idx-sn>1): #如果遇到栈顶
                        # here 计算入栈雨量
                        rainFall+=calculateRainFall(sn,idx,hi)
                stack.append((e,idx))

        # 2. 开始弹栈
        hit,snt = 0,0
        while (len(stack)>1):
            hi,sn = stack.pop()
            hit,snt =top(stack)
            # 计算出栈雨量
            if (sn-snt>1):
                rainFall += calculateRainFall(snt,sn,hi)


        return  rainFall


    def divide(self, dividend, divisor):
        """
        :type dividend: int
        :type divisor: int
        :rtype: int
        """

        sign = -1 if (divisor > 0) ^ (dividend > 0) else 1
        if abs(dividend) < abs(divisor): return 0
        if abs(dividend) == abs(divisor): return sign

        def recurvediv(r, n, bit):
            if r < n: return
            i = 0
            divsor = n
            maxval = 0
            while (divsor <= r):
                if divsor <= r:
                    i += 1
                    maxval = divsor
                divsor <<= 1

            r = r - maxval
            bit.append(i)
            recurvediv(r, n, bit)

        res = []
        result = 0
        recurvediv(abs(dividend), abs(divisor), res)
        for i in res:
            result += 2 ** (i - 1)
        result *= sign
        if result >= -1 * 2 ** 31 and result <= 2 ** 31 - 1:
            return result
        else:
            return 2 ** 31 - 1


    def mergeKLists(self, lists):
        """
        :type lists: List[ListNode]
        :rtype: ListNode
        """

        heap = []
        for ln in lists:
            if ln:
                heap.append((ln.val, ln))
        dummy = ListNode(0)
        cur = dummy
        heapq.heapify(heap)
        while heap:
            valu, ln_index = heapq.heappop(heap)
            cur.next = ln_index
            cur = cur.next
            if ln_index.next:
                heapq.heappush(heap, (ln_index.next.val, ln_index.next))
        return dummy.next





    def findKthLargest_heap(self, nums, k):
        length=len(nums)

        def LEFT(i):
            return 2*i

        def PARENT(i):
            return i//2

        def RIGHT(i):
            return 2*i+1

        def shift_down(A,i,length):  # 元素下沉算法
            l =LEFT(i)
            r =RIGHT(i)
            if l<= length and  A[l-1]<A[i-1]:
                minimum = l
            else:
                minimum =i
            if r<=length and A[r-1]<A[minimum-1]   :
                minimum = r

            if minimum != i :
                A[i-1],A[minimum-1] = A[minimum-1],A[i-1]
                shift_down(A,minimum,length)

        def build_min(A, k ):
            for i in range((k//2),0,-1):
                shift_down(A,i,k)

        # 1. 首先在原数组的基础上构建一个长度为K的最小堆
        build_min(nums,k)
        # 2. 从k+1开始
        for i in range(k+1,len(nums)+1):
            if nums[i-1]> nums[0]:
                nums[i-1],nums[0]=nums[0],nums[i-1]
                build_min(nums,k)

        return nums[0]
        pass


    def canFinish(self, numCourses, prerequisites):
        """
        :type numCourses: int
        :type prerequisites: List[List[int]]
        :rtype: bool
        """
        #先统计每个节点的出度和入度
        node = [[0,0] for i in range(numCourses)]
        node_edge = [[] for i in range(numCourses)]
        for u,v in prerequisites:
            node[v][1]+=1
            node[u][0]+=1
            node_edge[u].append([u,v])
            node_edge[v].append([u,v])


        while (0):
            print 's'

        pass




    def largestRectangleArea(self, heights):
        """
        :type heights: List[int]
        :rtype: int
        """
        def top(s):
            return s[-1] if len(s)>0 else (0,0)

        stack =[]
        i =1
        maxarea = 0
        for  item in heights:
            if len(stack)==0 or top(stack)<item:
                stack.append((item,i))
            else:
                while(top(stack)[0]>item):
                    hi,sn = stack.pop()
                    maxarea =max(maxarea,heights[sn-1]*(i-1-top(stack)[1]) )
                stack.append((item,i))

            i+=1

        while (len(stack)>0):
            hi,sn = stack.pop()
            maxarea = max(maxarea, heights[sn - 1] * (i - 1 - top(stack)[1]))

        return  maxarea


    def EnumerateTree1(self,n):
        """
            枚举树： 一般都要有一个循环嵌套的层数标志这里是layer 一个各个路径的状态标识位，
            一般采用路径的标识path ，这里的path[i] 中每个状态位既从左到右代表最外层到最内层的上的上的各个探索层次，
            同时也代表从头到尾的搜索路径。 一般采用两层循环，最外层代表各个搜索的层次，里面的循环代表每个当前搜索节点上的可用的选择路径（N叉选择树的遍历）
            :param n:
            :return:
        """

        path = [-1 for i in range(n)]
        layer = 0
        #得到每一层的起始数据

        while layer >=0 :
            iniLayerDat = path[layer]
            iniLayerDat+=1
            path[layer]=iniLayerDat
            #每次探索到尽头后原路返回
            if path[layer]>n:
                path[layer] =-1
                layer -=1
                continue
            if layer ==n-1:
                for i in range(n):
                    path[layer]+=1
                    print path
                #本层结束返回上一层
                path[layer] =-1
                layer-=1

            else:
                layer +=1




    def solveNQueens(self, n):
        """
        :type n: int
        :rtype: List[List[str]]
        """
        upper_limit = (1 << n) - 1

        def recursive(row, ld, rd, board):
            if row == upper_limit:
                self.ans.append(board)
            else:
                pos = upper_limit & (~(row | ld | rd))
                while pos:
                    p = pos & (~pos + 1)
                    pos -= p
                    tmp = board[:]
                    tmp.append(p)
                    recursive(row | p, (ld | p) << 1, (rd | p) >> 1, tmp)

        recursive(0, 0, 0, [])
        real_ans = []

        def trans(line):
            cnt = 0
            while line:
                line >>= 1
                cnt += 1
            return "." * (cnt - 1) + "Q" + "." * (n - cnt)

        for k in self.ans:
            tmp = []
            for j in k:
                tmp.append(trans(j))
            real_ans.append(tmp)
        return real_ans

    def solveNQueens1(self, n):
        """
        :type n: int
        :rtype: List[List[str]]
        """
        chessboard =[['.' for i in range(n)] for i in range(n)]
        res = []
        def recurseDfs(row,path,cols,level):
            #终止条件，得到了n个位置
            if len(path) == n :
                #print path
                rst =[]
                for s in chessboard:
                    rst.append(''.join(s))
                res.append(rst)
                return
            #for row in range(0 if len(path)==0 else path[-1][0],n):
            cols_set = set(cols)
            cols_remains = set(i for i in range(n))-cols_set
            for col in cols_remains:
                if    not isConflict(row,col,path):
                    chessboard[row][col]='Q' #放置一枚皇后棋子
                    path.append([row,col])
                    cols.append(col)
                    recurseDfs(row+1,path,cols ,level+1 )
                    chessboard[row][col] = '.'  # 进行回溯
                    path.pop()
                    cols.pop()


        def isConflict(i,j,path):
            # row is conflict ?
            for row ,col in  path:
                if row == i or col ==j :
                    return True
                if row +col == i+j or row -col == i -j:
                    return True
            return  False


        recurseDfs(0,[],[],0)
        return res




    def letterCombinations(self, digits):
        """
        :type digits: str
        :rtype: List[str]
         {'0', " "}, {'1',"*"}, {'2', "abc"},
            {'3',"def"}, {'4',"ghi"}, {'5',"jkl"},
            {'6',"mno"}, {'7',"pqrs"},{'8',"tuv"},
            {'9',"wxyz"}}
        """
        dict={'0':'','1':'*','2':'abc','3':'def','4':'ghi',
              '5':'jkl','6':'mno','7':'pqrs','8':'tuv','9':'wxyz'}
        res = []
        def dfs(path,level):
            if len(path)==len(digits):
                res.append(path)
                print path
                return
            for s in dict[digits[level]]:
                dfs( path+s,level+1)

        dfs('',0)
        return  res




    def EnumerateSum(self,candiate, target):
        """
        枚举树的求和
        leecode 39. 组合总和
        :param list:
        :param target:
        :return:
        """
        candiate.sort()
        length  = target//candiate[0]+1
        level=0
        #path[i]表示在第i个位置（树第i层）排list[path[i]],list的第p[i]个元素
        path=[-1 for i in range(length+2)]
        result = []
        remain =target
        while  level>=0:

            # 找到路径上没有用到最小（第一个）元素，起始从-1 递增寻找
            #例如：在【0，2，-1，-1，-1】 中从 -1，开始 -1,0,1 。1为最小的不存在于路径上的元素
            k = max(path[level] ,path[level-1]-1 if level>0 else path[level]) #start point of serching
            k+=1
            if k>=len(candiate):
                path[level]=-1
                level-=1
                continue


            path[level]=k
            p=0
            remain=target
            while path[p] > -1:
                remain-=candiate[path[p]]
                p+=1
            if remain<0:
                path[level]=-1

                level-=1
                continue


            if remain ==0 : #找到一个枚举的答案
                print "Enum:"
                res=[]
                p=0
                while path[p]>-1:
                    res.append(candiate[path[p]])
                    p+=1
                #print res
                result.append(res)
                print path
                remain += candiate[path[level]]

            else:
                level+=1

        return  result
        pass


    def EnumerateTree(self,list, n):
        """
        枚举树的迭代实现
        :param list:
        :param n:
        :return:
        """
        level=0
        #path[i]表示在第i个位置（树第i层）排list[path[i]],list的第p[i]个元素
        path=[-1 for i in range(n+2)]
        while  level>=0:

            # 找到路径上没有用到最小（第一个）元素，起始从-1 递增寻找
            #例如：在【0，2，-1，-1，-1】 中从 -1，开始 -1,0,1 。1为最小的不存在于路径上的元素
            k = path[level]  #start point of serching
            k+=1

            if k>n or level>n:
                for i in range(level,n+1):
                    path[i]=-1
                level-=1
                continue

            path[level]=k

            if level>=n : #找到一个枚举排列
                print "Enum:"
                res=[]
                for i in range(n+1):
                    res.append(list[path[i]])
                #print res
                print path
            else:
                level+=1

        return  0
        pass

    def Perm_iterative(self,list, n):
        """
        排列树的迭代实现
        :param list:
        :param n:
        :return:
        """
        level=0
        #path[i]表示在第i个位置（树第i层）排list[path[i]],list的第p[i]个元素
        path=[-1 for i in range(n+2)]
        while  level>=0:

            # 找到路径上没有用到最小（第一个）元素，起始从-1 递增寻找
            #例如：在【0，2，-1，-1，-1】 中从 -1，开始 -1,0,1 。1为最小的不存在于路径上的元素
            k = path[level]  #start point of serching
            while k in path: k+=1

            if k>n or level>n:
                for i in range(level,n+1):
                    path[i]=-1
                level-=1
                continue

            path[level]=k

            if level>=n : #找到一个排列
                print "11111"
                res=[]
                for i in range(n+1):
                    res.append(list[path[i]])
                print res
                print path
            else:
                level+=1

        return  0
        pass
    def combination(self,can,path):
        res =[]
        def dfs(path):
            if len(path) >= len(can):
                res.append(path)
                return

            for i in can:

                if path and i < path[-1]:
                    continue
                else:
                    dfs(path +[i])

        dfs([])
        return  res
        pass



    def combinationSum(self, candidates, target):
        """
        :type candidates: List[int]
        :type target: int
        :rtype: List[List[int]]
        """
        res = []
        candidates.sort()

        def dfs(remain, path):
            if not remain:
                res.append(path)
                return
            for i in candidates:
                if i > remain:
                    return
                elif path and i < path[-1]:
                    continue
                else:
                    dfs(remain - i, path + [i])

        dfs(target, [])
        return res
    def findroot(self,preorder,inorder,sublist):
        minorder ,order=0,0
        for i in sublist:
            for j in preorder:
                if i==j and order <minorder:
                    minorder = order
            order +=1
        return minorder



    def buildsubtree(self,root,preorder,inorder,s,e,pre_index):
        if s>e:
            return None,pre_index
        else:
            q = inorder.index(preorder[pre_index])
            root = TreeNode(inorder[q])
            pre_index +=1
            if len(preorder)>pre_index:
                root.left,pre_index = self.buildsubtree(root,preorder,inorder,s,q-1,pre_index)
                #if root.left:pre_index+=1
            if len(preorder)>pre_index:
                root.right ,pre_index=self.buildsubtree(root,preorder,inorder,q+1,e,pre_index)
            return  root,pre_index


    def buildTree(self, preorder, inorder):
        """
        :type preorder: List[int]
        :type inorder: List[int]
        :rtype: TreeNodesdfdfdsAAAAAAAa阿斯蒂芬sdfsdfdf啊实打实地方
        """


        if len(preorder) == 0 or len(inorder) == 0:
            return None


        root = TreeNode(0)
        root,index = self.buildsubtree(root,preorder,inorder,0,len(inorder)-1,0)
        return root


    def isSameTree(self, p, q):
        """
            :type p: TreeNode
            :type q: TreeNode
            :rtype: bool
        """
        if not p and not q: return True

        if (p ==None and q !=None) or (p!=None and q ==None ):
            return False
        # Both q and p is not None :
        if q.val != p.val:
            return False

        if not  self.isSameTree(p.right,q.right):
            return False

        if not  self.isSameTree(p.left,q.left):
            return False

        return True

        pass




    def minDepth(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        from collections import deque
        if not root: return 0
        q = deque()
        q.append((root, 0))
        level = 0

        while len(q) > 0:
            r, level = q.popleft()
            if r.left:
                q.append((r.left, level + 1))
            if r.right:
                q.append((r.right, level + 1))
            if r.left ==None and r.right==None:
                return level
        return  level

    def levelOrder(self, root):
        """
        :type root: TreeNode
        :rtype: List[List[int]]
        """
        from collections import deque
        if not root: return []
        q = deque()
        q.append((root, 0))
        res = [[], ]
        level = 0
        while len(q) > 0:
            r, level = q.popleft()
            print r.val
            if len(res) -1< level:
                res.append([])
            res[level].append(r.val)
            if r.left:
                q.append((r.left, level + 1))
            if r.right:
                q.append((r.right, level + 1))

        return res

    def listcreattree(self,root, llist, i):  ###用列表递归创建二叉树，
        # 它其实创建过程也是从根开始a开始，创左子树b，再创b的左子树，如果b的左子树为空，返回none。
        # 再接着创建b的右子树，
        if i < len(llist):
            if llist[i] == None:
                return None  ###这里的return很重要
            else:
                root = TreeNode(llist[i])
                print('列表序号：' + str(i) + ' 二叉树的值：' + str(root.val))
                # 往左递推
                root.left = self.listcreattree(root.left, llist, 2 * i + 1)  # 从根开始一直到最左，直至为空，
                # 往右回溯
                root.right = self.listcreattree(root.right, llist, 2 * i + 2)  # 再返回上一个根，回溯右，
                # 再返回根'
                print('************返回根：', root.val)
                return root  ###这里的return很重要
        return root





    def twoSum(self, nums, target):
        """
        :type nums: List[int]
        :type target: int
        :rtype: List[int]
        """
        hashmap = {}
        for index, num in enumerate(nums):
            another_num = target - num
            if another_num in hashmap:
                return [hashmap[another_num], index]
            hashmap[num] = index
        return None

    def __init__(self):
        self.index = 0
        self.ans = []

    def permute1(self,res,nums,s,e):
        if s ==e:
            print nums
            res.append([i for i in nums])
            return
        visited = set()
        for i in range(s,e+1):

            if nums[i] in visited:
                continue
            if i != s  and nums[s]==nums[i]:
                continue
            visited.add(nums[i])
            nums[s],nums[i]=nums[i],nums[s]
            self.permute1(res,nums,s+1,e)
            nums[s], nums[i] = nums[i], nums[s]

        pass

    def permute(self, nums):
        """
        :type nums: List[int]
        :rtype: List[List[int]]
        """
        l=[]
        self.permute1(l,nums,0,len(nums)-1)
        return  l
        pass

    def longestPalindrome(self, s):
        """
        :type s: str
        :rtype: str
        """
        st=list(s)
        l=len(s)
        result =''
        maxlen=0
        dp = [[False for i in range(l)] for j in range(l)]
        for j in range(l):
            for i in range(j+1):
                if i ==j:
                    dp[i][j]=True
                    if j - i + 1 >= maxlen:
                        result = st[i:j + 1]
                        maxlen = j - i + 1
                if j-i==1  and st[i]==st[j]: #长度2
                    dp[i][j]=True
                    if j-i+1>=maxlen   :
                        result = st[i:j+1]
                        maxlen =j-i+1
                if j-i>1 and st[i]==st[j]:
                    dp[i][j] =dp[i+1][j-1]
                    if j-i+1>=maxlen and dp[i][j]:
                        result = st[i:j+1]
                        maxlen =j-i+1
        return result








    def swapshift(self,s):#AkleBiCeilD
        i,j=0,0
        st =list(s)
        for chr in st:
            if chr.islower():
                k= j
                while(k>i):
                    st[k],st[k-1] =st[k-1],st[k]
                    k-=1
                i +=1
            j+=1
        print st
        return "".join(st)

        pass

    def intToChinese(self,n):
        r =''
        rep=['零', '一', '二', '三', '四', '五', '六', '七', '八', '九' ]
        unit =[ '', '十', '百', '千', '万','十','百' ,'千' ,'亿','十','百' ,'千']
        l =len(str(n))
        for i in range(0,l):
            #取最后位数值
            temp = n  % 10
            #取剩余位
            n =n/10

            if temp ==0:
                # 判断万位添加单位 万
                if i==4:
                    r+=unit[4]
                #判断亿位添加单位 亿
                if i==8:
                    r+=unit[8]
                #//判断当前最后位是否需要加 零
                if(r!='' and  r.decode('utf8')[len(r.decode('utf8'))-1].encode('utf8') != '零'
                         and  r.decode('utf8')[len(r.decode('utf8'))-1].encode('utf8') != '万'
                         and  r.decode('utf8')[len(r.decode('utf8'))-1].encode('utf8') != '亿'):
                    r += rep[temp];

            else:
                #当前位不是0 添加单位  添加数值
                r += unit[i];
                r += rep[temp];
        return r.decode('utf8')[::-1].encode('utf8')

    def romanToInt(self, s):
        """
        :type s: str
        :rtype: int
        L             50
        C             100
        D             500
        M             1000

        """
        dict={'I':1,'V':5,'X':10,'L':50,'C':100,'D':500,'M':1000}
        cacu = 0
        i=0
        while i<len(s):
            cur =s[i]
            lookhead =s[i+1 if i+1<len(s) else i]

            if dict[cur]<dict[lookhead]:
                res = dict[lookhead]-dict[cur]
                cacu +=res
                i+=2
            else:
                cacu +=dict[cur]
                i+=1

        return cacu


        pass

    def findKthLargest(self, nums, k):
        return self.select(nums,0,len(nums)-1,k)

        pass
    def swap(self,a,b):
        t=a
        a=b
        b=t

    def partition(self,A,p,r):
        pivol= A[r]
        i=p-1
        for j in range(p,r):
            if A[j]<=pivol:
                i += 1
                if i!=j:
                    A[j], A[i] =A[i],A[j]


        A[r],A[i+1]=A[i+1],A[r]
        return  i+1

    def random_partition(self,A,p,r):
        ran=int(p+ (r-p)*random.random())
        A[r],A[ran]=A[ran],A[r]
        return self.partition(A,p,r)



    def select(self,A,p,r,i):
        if p==r:
            return A[p]
        q= self.random_partition(A,p,r)
        k= q-p+1
        if k ==i :
            return A[q]
        elif i<k :
            return self.select(A,p,q-1,i)
        else:

            return  self.select(A,q+1,r,i-k)





    def minPathSum(self, grid):
        """
        :type grid: List[List[int]]
        :rtype: int
        """
        m = len(grid)
        n = len(grid[0])
        print(m, n)
        costi_1j, costij_1 = 0, 0
        cost = [[0 for i in range(n)] for i in range(m)]
        for i in range(0, m):
            for j in range(0, n):
                cost[i][j] = grid[i][j]
                print cost[i][j]
                if i == 0 and j == 0: continue
                costi_1j = cost[i - 1][j] if i > 0 else 99999
                costij_1 = cost[i][j - 1] if j > 0 else 99999
                print (costi_1j, costij_1)

                cost[i][j] += min(costi_1j, costij_1)

        print cost

        return cost[m - 1][n - 1]


    def rob(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        c=[0]*len(nums)
        c[0]=nums[0]
        c[1]=max(nums[0],nums[1])
        for i in range(2,len(nums)):
            c[i] =max(c[i-2] +nums[i],c[i-1])

        return c[len(nums)-1]



    def reverse(self, x):
        """
        :type x: int
        :rtype: int
        """
        if x<0 :
            flag =-1
        else:
            flag=1
        s=list(str(abs(x)))
        l=len(s)
        i =0
        while(i<(l-1-i)):
            tmp=s[i]
            s[i] =s[l-1-i]
            s[l-1-i] =tmp
            i+=1
        try:
            res = int("".join(s))
        except:
            return 0
        else:
            return res*flag


    def convert(self, s, numRows):
        """
        :type s: str
        :type numRows: int
        :rtype: str
        """
        """
                :type s: str
                :type numRows: int
                :rtype: str
        """
        num = list(range(1, numRows + 1)) + list(range(numRows - 1, 1, -1))
        n = len(num)
        m = len(s)
        k = m // n
        r = m % n

        seq = k * num + num[0:r]
        out = [""] * numRows
        for i in range(m):
            out[seq[i] - 1] += s[i]

        return ''.join(out)




    def lengthOfLongestSubstring(self, s):
        """
        :type s: str
        :rtype: int
        """
        head,tail =0,0
        subset ={}
        len =0
        for char in s :

            if char in subset and tail <subset[char]+1:
                tail =subset[char]+1
            subset[char] = head

            if head -tail +1>len:
                len =head -tail +1
            head += 1
        return len





        pass

    def majorityElement1(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        cnt ,num =0,0

        for n in nums:

            if n == num:
                cnt+=1
                continue
            if cnt ==0:
                num =n
                cnt+=1
                continue
            cnt-=1

        cnt= 0
        for n in nums:
            if num ==n:
                cnt+=1

        if cnt >= int(len(nums)/2)+(len(nums))%2:
            return num



        pass




    def majorityElement(self, nums):
        """
        :type nums: List[int]
        :rtype: List[int]
        """
        res1, res2 = 0, 0
        num1, num2 = 0, 0
        for num in nums:
            if num == res1:
                num1 += 1
                continue
            if res2 == num:
                num2 += 1
                continue
            if num1 == 0:
                res1 = num
                num1 += 1
                continue
            if num2 == 0:
                res2 = num
                num2 += 1
                continue
            num2 -= 1
            num1 -= 1
        res = []
        num1, num2 = 0, 0
        for num in nums:
            if num == res1:
                num1 += 1
            elif num == res2:
                num2 += 1
        threshold = len(nums) // 3
        if num1 > threshold:
            res.append(res1)
        if num2 > threshold and res1 != res2:
            res.append(res2)
        return res

    def isValid(self, s):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        stack = []

        # Hash map for keeping track of mappings. This keeps the code very clean.
        # Also makes adding more types of parenthesis easier
        hashmap ={')':'(','}':'{',']':'['}

        for c in s:
            if c in hashmap:
                if not stack : return False
                top = stack.pop()
                if top != hashmap[c]:
                    return False
                else:
                    continue

            stack.append(c)
        if  stack: return False

        return True
import json
def stringToListNode(input):
    # Generate list from the input
    numbers = json.loads(input)

    # Now convert that list into linked list
    dummyRoot = ListNode(0)
    ptr = dummyRoot
    for number in numbers:
        ptr.next = ListNode(number)
        ptr = ptr.next

    ptr = dummyRoot.next
    return ptr

def listNodeToString(node):
    if not node:
        return "[]"

    result = ""
    while node:
        result += str(node.val) + ", "
        node = node.next
    return "[" + result[:-2] + "]"

def makelist(lst):
    last = None

    for v in lst[::-1]:
        now =ListNode(v)
        now.next = last
        last =now
    return now

import copy
def reverseList(lst):
    pre = lst
    cur = lst.next
    pre.next = None
    while cur:
        nxt = cur.next
        cur.next = pre
        pre = cur
        cur = nxt
    return pre

head =ListNode(0)
def reverseListrecurse(lst,hds=None):
    """
    lst --prev lst.next--cur lst.next.next = cur.next
    :param lst:
    :return:
    """
    pre = lst
    cur = lst.next
    if pre.val == 0 :
        hds = pre

    if cur:
        print cur.val
        reverseListrecurse(cur,hds)
        if  cur.next ==None:
            #head1 = ListNode(0)
            hds.next =cur
            head.next  =cur
        cur.next  = pre if pre.val>0 else None  #cur.next=prev

def printl(l):
    while(l):
        print "%d,"%(l.val)
        l=l.next

def compversion(v1,v2):
    i1= strtoint(v1)
    i2= strtoint(v2)
    if i1>i2:
        return 1
    elif i1<i2 :
        return -1
    else:
        return 0
    pass

def strtoint(s):
    sa = s.split('.')
    r=0
    for i in range(0,4):
        r +=(int(sa[i]) if i<len(sa) else 0) *10**(3-i)

    return r

def main():
    import sys

    r= strtoint('1.0.1')
    r= compversion('0.1','1.1')
    t = TreeNode(0)
    l = TreeNode(0)
    r = TreeNode(0)
    #前序遍历
    preorder = [3,2,1,4]
    #中序遍历
    inorder = [1,2,3,4]
    testTrie()
    r = Solution().rotateRight(makelist([0,1,2]),4)
    r = Solution().trap([0,1,3,2,1,2,1])
    r =Solution().divide(3,1)
    r = Solution().mergeKLists([makelist([1,4,5]),makelist([1,3,4]),makelist([2,6])])
    r = Solution().findKthLargest_heap([3,2,3,1,2,4,5,5,6],4)
    r = Solution().canFinish(2,[[1,0],[0,1]])
    r = Solution().largestRectangleArea([2,1,5,6,2,3])
    r= Solution().EnumerateTree1(3)
    r = Solution().solveNQueens(5)
    print r
    r= Solution().letterCombinations('124')
    r= Solution().EnumerateSum([1,5,11],15)
    r = Solution().EnumerateTree  ([1, 2, 3, 4, 5], 2)
    r= Solution().Perm_iterative([1,2,3,4,5], 2)
    r = Solution().combination([2,3,5],[])

    t = Solution().buildTree(preorder,inorder)
    l = Solution().listcreattree(t, [1,2], 0)
    r = Solution().listcreattree(t, [1,None,2], 0)
    rs = Solution().isSameTree(l,r)
    t=Solution().listcreattree(t,[3,9,20,None,None,15,7],0)
    r= Solution().levelOrder(t)
    r=Solution().twoSum([0,2,4,0],0)
    r= Solution().permute([1,1,2,2])
    r=Solution().longestPalindrome('abcda')
    r=Solution().swapshift("hxKLAGLLzPyTxsFsrUnnSKQBHdQQrOyaEYJRgiJbHIDXFcQkFmIhPNK"
                           "IBfHxXDBdKAvgZiBLVwnlxJAHmttsSJkZhSmQneNVoKoIYZRjPq")
    print r
    r=Solution().intToChinese(2800006)
    r= Solution().romanToInt("MCMXCIV")
    f= Solution().findKthLargest([3,2,3,1,2,4,5,5,6],6)
    r=Solution().minPathSum([[1,2,5],[3,2,1]])
    r=Solution().rob([2,7,9,3,1])
    s= Solution().isValid('{()()}')
    s=Solution().convert('LEETCODEISHIRING',4)
    s= Solution().lengthOfLongestSubstring('abba')
    l = makelist([1,2,3,4])
    printl(l)
    reverseListrecurse(l)
    printl(l)
    reverseList(l)
    r= Solution().isValid('()())')

    print r

if __name__ == '__main__':
    main()