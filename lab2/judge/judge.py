try:
    import numpy as np
except ImportError:
    print("请先安装 numpy 库: pip install numpy")
    exit(1)

try:
    from numba import njit
except ImportError:
    print("请先安装 numba 库: pip install numba")
    exit(1)

try:
    import edlib
except ImportError:
    print("请先安装 edlib 库: pip install edlib")
    exit(1)


def get_rc(s):
    map_dict = {'A': 'T', 'T': 'A', 'G': 'C', 'C': 'G', 'N': 'N'}
    l = []
    for c in s:
        l.append(map_dict[c])
    l = l[::-1]
    return ''.join(l)
def rc(s):
    map_dict = {'A': 'T', 'T': 'A', 'G': 'C', 'C': 'G', 'N': 'N'}
    l = []
    for c in s:
        l.append(map_dict[c])
    l = l[::-1]
    return ''.join(l)

def seq2hashtable_multi_test(refseq, testseq, kmersize=15, shift = 1):
    rc_testseq = get_rc(testseq)
    testseq_len = len(testseq)
    local_lookuptable = dict()
    skiphash = hash('N'*kmersize)
    for iloc in range(0, len(refseq) - kmersize + 1, 1):
        hashedkmer = hash(refseq[iloc:iloc+kmersize])
        if(skiphash == hashedkmer):
            continue
        if(hashedkmer in local_lookuptable):

            local_lookuptable[hashedkmer].append(iloc)
        else:
            local_lookuptable[hashedkmer] = [iloc]
    iloc = -1
    readend = testseq_len-kmersize+1
    one_mapinfo = []
    preiloc = 0
    while(True):
   
        iloc += shift
        if(iloc >= readend):
            break

        #if(hash(testseq[iloc: iloc + kmersize]) == hash(rc_testseq[-(iloc + kmersize): -iloc])):
            #continue
 
        hashedkmer = hash(testseq[iloc: iloc + kmersize])
        if(hashedkmer in local_lookuptable):

            for refloc in local_lookuptable[hashedkmer]:

                one_mapinfo.append((iloc, refloc, 1, kmersize))



        hashedkmer = hash(rc_testseq[-(iloc + kmersize): -iloc])
        if(hashedkmer in local_lookuptable):
            for refloc in local_lookuptable[hashedkmer]:
                one_mapinfo.append((iloc, refloc, -1, kmersize))
        preiloc = iloc

    

    return np.array(one_mapinfo)

def get_points(tuples_str):
    data = []
    num = 0
    for c in tuples_str:
        if(ord('0') <= c <= ord('9')):
            num = num * 10 + c - ord('0')
        elif(ord(',') == c):
            data.append(num)
            num = 0
    if(num != 0):
        data.append(num)
    return data

def calculate_distance(ref, query, ref_st, ref_en, query_st, query_en):
    A = ref[ref_st: ref_en]
    a = query[query_st: query_en]
    _a = rc(query[query_st: query_en])
    return min(edlib.align(A, a)['editDistance'], edlib.align(A, _a)['editDistance'])

def get_first(x):
    return x[0]


def calculate_value(tuples_str, ref, query):  

    slicepoints = np.array(get_points(tuples_str.encode()))
    if(len(slicepoints) > 0 and len(slicepoints) % 4 == 0):
        editdistance = 0
        aligned = 0
        preend = 0
        points = np.array(slicepoints).reshape((-1, 4)).tolist()
        points.sort(key=get_first)
        for onetuple in points:
            query_st, query_en, ref_st, ref_en = onetuple
            if(preend > query_st):
                return 0
            if(query_en - query_st < 30):
                continue
            preend = query_en
            if((calculate_distance(ref, query, ref_st, ref_en, query_st, query_en)/len(query[query_st:query_en])) > 0.1):
                continue
            editdistance += calculate_distance(ref, query, ref_st, ref_en, query_st, query_en)
            aligned += len(query[query_st:query_en])
        return max(aligned - editdistance, 0)
    else:
        return 0


if __name__ == "__main__":
    import os
    import sys
    
    # 测试文件1
    print("测试文件1的得分：")
    ref_file1 = os.path.join(os.path.dirname(os.path.dirname(__file__)), "sequence", "reference1.in")
    query_file1 = os.path.join(os.path.dirname(os.path.dirname(__file__)), "sequence", "query1.in")
    result_file1 = os.path.join(os.path.dirname(os.path.dirname(__file__)), "result", "result1.out")
    
    # 读取reference文件
    with open(ref_file1, "r") as f:
        ref1 = f.read().strip()
    
    # 读取query文件
    with open(query_file1, "r") as f:
        query1 = f.read().strip()
    
    # 读取result文件
    with open(result_file1, "r") as f:
        result1 = f.read().strip()
    
    # 计算得分
    score1 = calculate_value(result1, ref1, query1)
    print(f"文件1得分: {score1}")
      # 测试文件2
    print("\n测试文件2的得分：")
    ref_file2 = os.path.join(os.path.dirname(os.path.dirname(__file__)), "sequence", "reference2.in")
    query_file2 = os.path.join(os.path.dirname(os.path.dirname(__file__)), "sequence", "query2.in")
    result_file2 = os.path.join(os.path.dirname(os.path.dirname(__file__)), "result", "result2.out")
    
    # 读取reference文件
    with open(ref_file2, "r") as f:
        ref2 = f.read().strip()
    
    # 读取query文件
    with open(query_file2, "r") as f:
        query2 = f.read().strip()
    
    # 读取result文件
    with open(result_file2, "r") as f:
        result2 = f.read().strip()
    
    # 计算得分
    score2 = calculate_value(result2, ref2, query2)
    print(f"文件2得分: {score2}")

