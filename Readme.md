# llvm的后缀树算法库使用说明

输入：无符号整数数组

输出：数组中重复出现的指令序列的集合

## 主要流程

- 生成无符号整数数组
- 构造后缀树
- 获取重复指令序列信息
- 消除交叠情况
  - 交叠：字符串“abcab$”中子串“abc”和子串“cab”交叠
  - 消除方法：
    - 先按照收益排序
      - 具体的收益模型为函数 `unsigned RepeatedInfos::RepeatedSubstringByS::getPredictBenefit`
      - 可以根据需求修改收益模型
    - 在发现某一子序列和先前序列交叠时，删除当前子序列
- 评估整体冗余