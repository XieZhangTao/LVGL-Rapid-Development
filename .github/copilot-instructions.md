---
applyTo: "**"
description: "GitHub Copilot instructions to enhance code suggestions with context-aware guidance."
---
# Copilot 代码规范

## 通用编程规范

### 变量命名规范

- 使用有意义且描述性的变量名，避免使用单字母变量名（如 a、b、c），除非在循环中作为索引使用。
### 变量命名规范

- 使用有意义且描述性的变量名，避免使用单字母变量名（如 a、b、c），除非在循环中作为索引使用。
- 命名格式（与仓库现有代码一致）：
  - 可选作用域前缀：`s_`（文件/静态作用域）、`g_`（全局作用域）。
  - 紧随其后使用类型缩写作为前缀（例如 `t`、`ch`、`hw`、`w`、`f`、`b`、`p` 等），然后使用驼峰风格的描述性名称（仓库中常见为首字母大写的风格）。
  - 格式示例：`s_tBtnPool`、`s_chBtnPoolIndex`、`pchValue`、`bIsVisible`。
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
   - 对于复合类型变量，使用多个类型缩写组合在一起，按顺序排列。例如：`pchValue`（p 表示 pointer，ch 表示 uint8_t）。
   - 布尔变量：仓库中常使用 `b` 前缀（例如 `bPinLevel`），也可使用 `is`/`has` 前缀作为可读性替代。
   - 全局或静态变量：使用 `g_` 或 `s_` 前缀并用下划线与后续部分隔开，后续名称仍使用类型前缀 + 驼峰，例如：`s_fDefaultTimeout`、`s_tBtnFifo`。
   - 宏定义：全部使用大写字母，单词之间用下划线 `_` 分隔，例如：`#define MAX_BUFFER_SIZE 1024`。
   - 例外：对于短期循环索引，使用 `i`、`j`、`k` 等。
- 宏定义命名应全部使用大写字母，单词之间用下划线_分隔。
  - 例如：`#define MAX_BUFFER_SIZE 1024`
- 例外情况：
  - 对于循环变量，可以使用更简短的名称，如 i、j、k 等。

### 函数命名规范

- 函数名应采用蛇形命名法，所有字母小写，单词之间用下划线 _连接。
- 函数名字格式使用 对象/模块 + 事件 + 动作 的格式，以提高可读性和一致性。
  - 例如：`emb_btn_event_get`、`emb_btn_event_clear`。
- 对于返回布尔值的函数，函数名则以 模块_is/has/can/shoulud..._动词 的格式命名。
  -状态判断：is_xxx
  包含判断：has_xxx
  能力判断：can_xxx
  策略判断：should_xxx
  - 例如：`file_is_valid`、`network_has_permission`。

- 私有实现：对于文件内部的私有函数，允许使用双下划线前缀并将其声明为 `static` 或 `static inline`，例如 `__emb_btn_emit`。

### 代码风格规范

- 代码缩进应使用四个空格；避免在代码缩进中使用制表符（Tab）。注释内部的制表符用于对齐的历史内容可以保留，但新增代码请使用空格对齐。
- 每行代码长度一般不超过 80 个字符，如果超出长度小于15个字符，可以接受在同一行，超出太多则应换行处理。
  - 例如：`    uint16_t hwFreeCount = (uint16_t)(ptThis->tCFG.hwBufferSize - (uint16_t)(wHead - wTail));` 可以接受在同一行。
- 对于函数调用，如果参数过多导致行长度超过 80 个字符，应将每个参数放在单独的一行，同一个参数不应该占据多行，并且每个参数都应该左对齐。
  - 例如：
    ```c
        memcpy((uint8_t *)pvData + (uint32_t)hwSegment1 * ptThis->tCFG.hwElementSize,
               pchBuffer,
               (uint32_t)hwSegment2 * ptThis->tCFG.hwElementSize);
    ```
- 使用大括号 {} 明确代码块的开始和结束，即使代码块只有一行代码。
- 在运算符两侧添加空格，以提高代码的可读性。
  - 例如：`a + b`、`x == y`。
- 在判断条件中使用括号来明确表达式的优先级。
  - 例如：`if((a > b) && (c < d))`。
- 在相等比较中，常量应放在变量的右侧，以避免误用赋值运算符。
  - 例如：`if(10 == a)` 而不是 `if(a == 10)`。
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
- 在数组初始化时需要严格对齐，确保代码整齐美观。使用宽度对齐，使得每个元素的相同部分在同一列对齐，1日文=1中文=2英文=2俄文（字符），宽度不一致的同一列使用空格对齐到同一宽度，增强代码的可读性。
  - 例如：
    ```c
      static const char* const FONT_SETTING_POWER[][5] = {
          { "暗屏时间" , "暗屏時間" , "Dim Time"       , "減光時間"      , "Время затемнения" },
          { "关机时间" , "關機時間" , "Power Off Time" , "電源オフ時間"  , "Время выключения" },
          { "5分钟"    , "5分鐘"    , "5 min"          , "5分"           , "5 мин"            },
          { "10分钟"   , "10分鐘"   , "10 min"         , "10分"          , "10 мин"           },
          { "30分钟"   , "30分鐘"   , "30 min"         , "30分"          , "30 мин"           },
          { "从不"     , "從不"     , "Never"          , "しない"        , "Никогда"          },
          { "10分钟"   , "10分鐘"   , "10 min"         , "10分"          , "10 мин"           },
          { "30分钟"   , "30分鐘"   , "30 min"         , "30分"          , "30 мин"           },
          { "1小时"    , "1小時"    , "1 hour"         , "1時間"         , "1 час"            },
          { "从不"     , "從不"     , "Never"          , "しない"        , "Никогда"          },
      };
    ```
- 调用相同的函数时入参需要严格对齐，确保代码整齐美观。
  - 例如：
    ```c
      emb_assert(ptDirtyRegion != NULL);
      emb_assert(ptNewRegion   != NULL);

      lv_obj_align(ptIcon[0] , LV_ALIGN_TOP_LEFT, 11, 4);
      lv_obj_align(ptIcon[1] , LV_ALIGN_TOP_LEFT, 4 , 4);
      lv_obj_align(ptIcon[2] , LV_ALIGN_TOP_LEFT, 4 , 4);
      lv_obj_align(ptIcon[30], LV_ALIGN_TOP_LEFT, 4 , 4);
    ```
- 条件语句的多行表达式，如果过长需要换行，换行后的代码需要严格对齐，且符号需要放置在行首，确保代码整齐美观。
  - 例如：
    ```c
      if (    (ptRegion1->tLocation.iX >= ptRegion0->tLocation.iX)
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
- 在.h文件中声明函数，需要使用extern，且extern在函数定义时另起一行，如果函数有其他属性，例如static或inline，也应另起一行，确保代码整齐美观。
  - 例如：
    ```c
    extern 
    static inline
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
- 每个函数添加步骤注释，描述函数的主要步骤和逻辑。每个步骤函数间应该间隔一行。
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

## LVGL相关规范

- 在使用LVGL时，默认使用对齐函数 `lv_obj_align` 来设置对象的位置。
- 当我使用对齐函数（如 `lv_obj_align`）时，参数默认为 `LV_ALIGN_TOP_LEFT`，除非我明确指定其他对齐方式。
  - 例如：`lv_obj_align(obj, LV_ALIGN_TOP_LEFT, x_offset, y_offset);`

## 语言偏好

- 无论我用什么语言提问，你必须用中文回答，除非我明确要求你用别的语言回答。

## 行为准则

- 不要跟我客套，直接回答我的问题。
- 如果有其他建议，不需要等我问，直接告诉我。
- 直接应用这些规范来生成代码，不需要我每次都提醒你。
- 你可以直接帮我生成代码片段，不需要我每次都说“请帮我生成代码”。
