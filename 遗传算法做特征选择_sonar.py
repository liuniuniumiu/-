#!/usr/bin/env python
# coding: utf-8

# In[10]:


import pandas as pd
from pandas import DataFrame,Series
import numpy as np
from IPython.core.interactiveshell import InteractiveShell
InteractiveShell.ast_node_interactivity = "all"


# In[11]:


sonar=pd.read_csv('sonar.data')
sonar_data=sonar.iloc[:,0:60]
sonar_label=sonar.iloc[:,60:]


# In[3]:


#适应度计算
def get_J(data):
    data=np.mat(data)
    u=data.mean(0)
    cla_f=data[0:97]
    cla_s=data[97:]
    u_f=cla_f.mean(0)
    u_s=cla_s.mean(0)
    Sb=97/208*(u_f-u)*(u_f-u).T+101/208*(u_s-u)*(u_s-u).T
    S1,S2=np.mat(0.),np.mat(0.)
    for i in range(97):
        S1+=(cla_f[i]-u_f)*(cla_f[i]-u_f).T
    S1=97/208*S1/97
    for i in range(101):
        S2+=(cla_s[i]-u_s)*(cla_s[i]-u_s).T
    S2=101/208*S2/101
    Sw=S1+S2
    J=np.trace(Sw+Sb)
    return J


# In[2]:


def cal_J(pop):
    #计算各个个体适应度,返回个体的适应度数组
    J_array=np.zeros(31)
    for i in range(31):
        one_loc=np.where(pop[i]==1)[0]
        J_array[i]=get_J(sonar_data.iloc[:,one_loc])
    return J_array


# In[1]:


def initial_pop(select_k):
    import random
    #初始化种群
    pop=np.zeros((31,60))
    arange=np.arange(0,31)
    arange=arange.tolist()
    for i in range(31):
        select_one=random.sample(arange,select_k)
        pop[i][select_one]=1
    return pop


# In[14]:


#选择
def select(pop,J_array,select_k):
    #轮盘赌选择
    org_J_sort=J_array.argsort() #记录原始个体适应度排名
    tem_J_array=J_array/J_array.max() #类似归一化
    tem_J_array=sorted(tem_J_array)
    tem_J_array=np.array(tem_J_array)
    min_probal=sorted(J_array/J_array.sum())[0] #降低最小适应度个体被选择的可能性
    select_num=np.zeros(30)
    counter=0
    while(True):
        num=np.random.random_sample(1)
        if num>=(tem_J_array[0]-min_probal):
            select_num[counter]=num
            counter+=1
        if counter>29:
            break
    one=np.array([1.])
    select_num=np.concatenate((select_num,one)) #得到含有1的31个[0,1]的随机数
    for i in range(31):
        probal=select_num[i]
        for j in range(31):
            if probal<=tem_J_array[j]:
                select_num[i]=j
                break
    select_num=select_num.astype('int32') #得到31个随机数在轮盘中的排名
    for i in range(31):
        select_num[i]=org_J_sort[select_num[i]] #得到31个被选中的随机个体（始终保留最大适应度个体）
    new_pop=np.zeros((31,60))
    for i in range(31):
        new_pop[i]=pop[select_num[i]] #得种群
    return new_pop


# In[6]:


#交叉
import math
#得到两个交叉个体的交叉位
def get_cross_bit(indiv_1,indiv_2):
    list_1_0=[] #indiv1为1，indiv2为0的索引列表
    list_0_1=[] #indiv1为0，indiv2为1的索引列表
    for i in range(60):
        if indiv_1[i]==1 and indiv_2[i]==0:
            list_1_0.append(i)
        elif indiv_1[i]==0 and indiv_2[i]==1:
            list_0_1.append(i) #不对两个个体中都为1的基因位交叉
    cross_lenth=math.floor((min([len(list_1_0),len(list_0_1)])+1)/2) #交叉长度为小值加1除以2取整
    a=min([len(list_1_0),len(list_0_1)])-cross_lenth
    if min([len(list_1_0),len(list_0_1)])-cross_lenth<=0:
        a=1
    start_bit=np.random.randint(0,a,1)
    list_1_0=np.array(list_1_0)
    list_0_1=np.array(list_0_1)
    return list_1_0[start_bit[0]:(start_bit+cross_lenth)[0]],list_0_1[start_bit[0]:(start_bit+cross_lenth)[0]]

#选出交叉个体，进行交叉
def cross(pop,J_array,select_k):
    counter=0
    #两项交叉得到新的交叉后的种群（子代平均适应度大于父代）
    while(True):
        if counter>50:
            break
        import random
        #得到交叉项
        best_indiv=(-J_array).argsort()[0] #选择种群最佳个体
        cross_num=np.arange(31)
        cross_num=np.delete(cross_num,best_indiv)
        cross_num=cross_num.tolist()
        cross_1=random.sample(cross_num,15)
        cross_1=np.array(cross_1) #15个交叉第一项
        cross_num=np.array(cross_num)
        cross_2=[]
        for i in cross_num:
            if i not in cross_1:
                cross_2.append(i)
        cross_2=np.array(cross_2) #15个交叉第二项
        old_pop=pop
        for i in range(15):
            cross_bit_1,cross_bit_2=get_cross_bit(pop[cross_1[i]],pop[cross_2[i]]) #每两个个体的交叉位，cross_bit_1为第一项为1的项
            if cross_bit_1.size!=0 and cross_bit_2.size!=0:
                pop[cross_1[i]][cross_bit_1]=0
                pop[cross_1[i]][cross_bit_2]=1
                pop[cross_2[i]][cross_bit_2]=0
                pop[cross_2[i]][cross_bit_1]=1 #交叉完毕
        new_J_array=np.zeros(31)
        for i in range(31):
            one_loc=np.where(pop[i]==1)[0]
            new_J_array[i]=get_J(sonar_data.iloc[:,one_loc]) #得到交叉后种群各个个体的适应度
        if new_J_array.mean()>J_array.mean(): #要求子代平均适应度大于父代
            break 
        else:
            pop=old_pop
        counter+=1 #不大于则保持旧种群循环，直到子代平均适应度大于父代
    return pop,new_J_array


# In[7]:


#变异
def varia(pop,J_array,select_k):
    best_indiv=(-J_array).argsort()[0] #选择种群最佳个体
    for i in range(31):
        if i!=best_indiv:
            find_one=np.where(pop[i]==1)[0]
            find_two=np.where(pop[i]==0)[0] #找到0,1位置
            pro_one=np.random.randint(0,select_k,1)
            pro_zero=np.random.randint(0,60-select_k,1) #随机选择位数
            pop[i][find_one[pro_one]]=0
            pop[i][find_two[pro_zero]]=1 #变异
    return pop


# In[24]:


if __name__ == '__main__':
    every_max=np.zeros(10)
    for k in range(1,11):
        select_k=k
        pop=initial_pop(select_k)
        J_array_max=np.zeros(20)
        counter,i=0,0
        J_array_max_step=np.zeros(200)
        while(True):
            if counter%20==0 and counter!=0:
                i=0
            J_array=cal_J(pop)
            J_array_max[i]=J_array.max()
            J_array_max_step[counter]=J_array_max[i] #记录每次迭代最大值
            pop=select(pop,J_array,select_k)
            J_array=cal_J(pop)
            pop,J_array=cross(pop,J_array,select_k)
            pop=varia(pop,J_array,select_k)
            if np.where(J_array_max==J_array_max[0])[0].shape[0]==20 or counter>=200: #20次最大值不变或迭代超过200次，停止迭代
                print('epoch{},end！'.format(counter+1))
                break
            if (counter+1)%40==0:
                print('epoch{},please wait!'.format(counter+1))
            counter+=1
            i+=1
        every_max[k-1]=J_array_max_step[counter]


# In[25]:


every_max


# In[16]:


final_indiv=pop[J_array.argsort()[0]]
selected_char=[]
for i in range(60):
    if final_indiv[i]==1:
        selected_char.append(i)
print('被选中的特征维数为：{}'.format(selected_char))
print('J={}'.format(J_array_max_step[counter]))


# In[26]:


import matplotlib.pyplot as plt
x=np.arange(0,counter)
fig=plt.figure(figsize=(8,6))
plt.plot(x,J_array_max_step[0:counter],c='orange',label='max_J')
plt.legend()
plt.grid(True)
plt.xlabel('epoch',fontsize=10,color='c')
plt.ylabel('J_num',fontsize=10,color='c')
plt.title('GA record',fontsize=15)
plt.show()


# In[30]:


fig=plt.figure(figsize=(8,6))
x=np.arange(1,11)
plt.plot(x,every_max,color='b',label='J')
plt.grid(True)
plt.title('for 1 to 10',fontsize=15)
plt.xlabel('characteristic number',fontsize=8)
plt.ylabel('J_num',fontsize=8)
plt.legend()

