---
applyTo: "**"
description: "GitHub Copilot instructions to enhance code suggestions with context-aware guidance."
---

# Copilot 代码规范

## 通用编程规范

### 变量命名规范
- 使用有意义且描述性的变量名，避免使用单字母变量名（如 a、b、c），除非在循环中作为索引使用。
- 变量名应采用优化后的小驼峰命名法（camelCase），第一个单词为变量类型缩写，后面每个单词的首字母大写，其余字母小写。单词之间没有分隔符。
    - 例如：温度变量命名为 `fTemperature`（f 表示 float 类型）。
- 常见类型缩写：
    - int8_t：i8
    - int16_t：i16
    - int32_t：i32
    - uint8_t：ch
    - uint16_t：hw
    - uint32_t：w
    - 浮点数（Float）：f
    - 布尔值（Boolean）：b
    - 字符串（String）：ch
    - 结构体（Struct）：t
    - 指针（Pointer）：p
    - 函数指针（Function）：pfn
- 对于复合类型变量，使用多个类型缩写组合在一起，按顺序排列。
    - 例如：指向uint8_t的指针命名为 `pchValue`（p 表示 pointer，ch 表示 uint8_t）。
- 布尔变量命名应以 is、has、can、should 等动词开头，以明确表示其布尔性质。
    - 例如：`bIsVisible`、`bHasChildren`。
- 全局变量或者静态变量需要在遵从小驼峰命名法的基础上，添加前缀以表示其作用域或用途，命名应以 g 或 s 开头，并且用下划线_与后面的小驼峰命名变量名隔开。
    - 例如： `g_chMaxCount`、`s_fDefaultTimeout`。
- 宏定义命名应全部使用大写字母，单词之间用下划线_分隔。
    - 例如：`#define MAX_BUFFER_SIZE 1024`
- 例外情况：
    - 对于循环变量，可以使用更简短的名称，如 i、j、k 等。

### 函数命名规范
- 函数名应采用蛇形命名法，所有字母小写，单词之间用下划线 _连接。
- 函数名字格式使用 模块_动词_名词 的格式，以提高可读性和一致性。
    - 例如：`file_read_data`、`network_connection_establish`。
- 对于返回布尔值的函数，函数名则以 模块_is/has/can/shoulud..._动词 的格式命名。
    - 例如：`file_is_valid`、`network_has_permission`。

### 代码风格规范
- 使用四个空格进行缩进，禁止使用制表符（Tab）。
- 每行代码长度不超过 80 个字符，超过部分应换行处理。
- 使用大括号 {} 明确代码块的开始和结束，即使代码块只有一行代码。
- 在运算符两侧添加空格，以提高代码的可读性。
    - 例如：`a + b`、`x == y`。
- 在判断条件中使用括号来明确表达式的优先级。
    - 例如：`if ((a > b) && (c < d))`。
- 在相等比较中，常量应放在变量的右侧，以避免误用赋值运算符。
    - 例如：`if (a == 10)` 而不是 `if (10 == a)`。
- 在关键字（如 if、for、while、switch 等）和左括号之间添加空格。
    - 例如：`if (condition)`、`for (int i = 0; i < n; i++)`。
- 函数参数列表中的逗号后应添加空格。
    - 例如：`function_name(param1, param2, param3)`。
- 在文件末尾添加一个空行。
- 变量声明时需要严格对齐，确保代码整齐美观。
    - 例如：
      ```c
        static emb_fifo_t s_tBtnFifo;
        static uint8_t    s_chBtnFifoBuffer[__EMB_BTN_FIFO_SIZE__];
        static emb_btn_t  s_tBtnPool[__EMB_BTN_MAX_COUNT__];
        static uint8_t    s_chBtnPoolIndex = 0;
      ```
- 结构体声明时需要严格对齐，确保代码整齐美观。
    - 例如：
      ```c
        typedef struct{
        emb_btn_cfg_t       tCFG              ; 
        uint16_t            u12Tick       : 12; 
        uint8_t             u4FilterTimer :  4; 
        uint8_t             chMask            ; 
        emb_btn_state_t     tState        :  4; 
        uint8_t             bPinLevel     :  1; 
        uint8_t             u3ClickCount  :  3; 
        emb_btn_callback_t  pfCallback[6]     ; 
        } emb_btn_t;
      ```
- 在数组初始化时需要严格对齐，确保代码整齐美观。使用宽度对齐，使得每个元素的相同部分在同一列对齐，1中文=2英文，宽度不一致的同一列使用空格对齐到同一宽度，增强代码的可读性。
    - 例如：
      ```c
        static const char * const FONT_SETTING_DETAIL[][3] = {
          {"过压保护值:"  ,   "過壓保護值:"  ,  "OVP Value:"    },
          {"过流保护值:"  ,   "過流保護值:"  ,  "OCP Value:"    },
          {"过功率保护值:",   "過功率保護值:",  "OPP Value:"    },
          {"过热保护值:"  ,   "過熱保護值:"  ,  "OTP Value:"    },
          {"开启"         ,   "開啟"         ,  "On"            },
          {"关闭"         ,   "關閉"         ,  "Off"           },
          {"亮度:"        ,   "亮度:"        ,  "Brightness:"   },
          {"暗屏时间:"    ,   "暗屏時間:"    ,  "Dim Time:"     },
          {"低"           ,   "低"           ,  "Low"           },
          {"中"           ,   "中"           ,  "Mid"           },
          {"高"           ,   "高"           ,  "High"          },
          {"简体中文"     ,   "简体中文"     ,  "简体中文"      },
          {"繁体中文"     ,   "繁体中文"     ,  "繁体中文"      },
          {"English"      ,   "English"      ,  "English"       },
          {"蓝牙开关"     ,   "蓝牙开关"     ,  "Bluetooth"     },
          {"恢复出厂"     ,   "恢復出廠"     ,  "Factory Reset" },
          {"使用说明"     ,   "使用說明"     ,  "Manual"        },
          {"厂商:"        ,   "廠商:"        ,  "Manufacturer:" },
          {"型号:"        ,   "型號:"        ,  "Model:"        },
          {"引导版本:"    ,   "引導版本:"    ,  "Boot Ver:"     },
          {"硬件版本:"    ,   "硬件版本:"    ,  "HW Ver:"       },
          {"软件版本:"    ,   "軟件版本:"    ,  "SW Ver:"       }
      };
      ```
- 函数调用时需要严格对齐，确保代码整齐美观。
    - 例如：
      ```c
        emb_assert(ptDirtyRegion != NULL);
        emb_assert(ptNewRegion   != NULL);
      ```
- 条件语句的多行表达式，如果过长需要换行，换行后的代码需要严格对齐，且符号需要放置在行首，确保代码整齐美观。
    - 例如：
      ```c
        if (   (ptRegion1->tLocation.iX >= ptRegion0->tLocation.iX)
            && (ptRegion1->tLocation.iY >= ptRegion0->tLocation.iY)
            && (iRegionEndX1  <= iRegionEndX0)
            && (iRegionEndY1  <= iRegionEndY0))
      ```
- 函数定义时如果参数过多需要换行，换行后的代码需要严格对齐，确保代码整齐美观。
    - 例如：
      ```c
        void _emb_2d_region_init(emb_2d_region_t *ptRegion,
                                 int16_t iX,
                                 int16_t iY,
                                 int16_t iWidth,
                                 int16_t iHeight)
      ```
- 在.h文件中声明函数，需要使用extern，且extern在函数定义时另起一行，确保代码整齐美观。
    - 例如：
        ```c
        extern 
        void _emb_2d_region_init(emb_2d_region_t *ptRegion,
                                 int16_t iX,
                                 int16_t iY,
                                 int16_t iWidth,
                                 int16_t iHeight);
        ```

### 注释规范
- 所有注释应使用英文且简短，确保国际化团队成员能够理解。
- 所有注释如果过长，均需换行处理，换行后的注释内容需要严格对齐，确保代码整齐美观。
    - 例如：
        ```c
        /* This is a long comment that needs to be wrapped
           to the next line for better readability and
           alignment in the code.                         */
        ```
- 函数的注释在.c与.h文件中均需存在，且内容应保持一致，但是风格不一样。
    - .c文件中：
        ```c
        /*******************************************************************************
        * @brief  dequeue data from the fifo struct
        * @param  ptFIFO: pointer of the fifo struct
        * @param  pvData: pointer of the data to be dequeued
        * @return 0: dequeue error
        *         1: dequeued one data
        *******************************************************************************/
        uint16_t _emb_fifo_dequeue2(emb_fifo_t *ptFIFO, void *pvData)
        ```
    - .h文件中：
        ```c
        /**
         * @brief  dequeue data from the fifo struct
         * @param  ptFIFO: pointer of the fifo struct
         * @param  pvData: pointer of the data to be dequeued
         * @return 0: dequeue error
         *         1: dequeued one data
         */
        uint16_t _emb_fifo_dequeue2(emb_fifo_t *ptFIFO, void *pvData)
        ```
- 每个函数添加步骤注释，描述函数的主要步骤和逻辑。
    - 例如：
        ```c
        uint16_t _emb_fifo_dequeue2(emb_fifo_t *ptFIFO, void *pvData)
        {
            emb_assert(ptFIFO != NULL && pvData != NULL);
            /*	step1,	check if the fifo is empty	       */
            emb_fifo_t *ptThis = ptFIFO;
            if(emb_fifo_is_empty(ptThis)) return 0;

            /*	step2,	calculate the read position        */
            uint8_t *pchBuffer = (uint8_t *)ptThis->tCFG.pvBuffer;
            pchBuffer += (FIFO_TAIL(ptThis) * ptThis->tCFG.hwElementSize);

            /*	step3,	read the data from the buffer      */
            emb_memcpy(pvData, pchBuffer, ptThis->tCFG.hwElementSize);

            /*	step4,	update the tail pointer		       */
            ptThis->wTail++;

            EMB_LOG_INFO(
                FIFO,
                "dequeue 1 element"
            );

            return 1;
        }
        ```

## 语言偏好
- 无论我用什么语言提问，你必须用中文回答，除非我明确要求你用别的语言回答。

## 行为准则
- 不要跟我客套，直接回答我的问题。
- 如果有其他建议，不需要等我问，直接告诉我。
- 直接应用这些规范来生成代码，不需要我每次都提醒你。
- 你可以直接帮我生成代码片段，不需要我每次都说“请帮我生成代码”。