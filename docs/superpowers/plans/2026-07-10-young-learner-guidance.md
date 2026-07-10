# 低龄学员引导与危险工具安全 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 让项目介绍只讲玩法，开始前完成三题理解检查，危险工具始终由家长指导并陪同，同时把孩子每轮阅读量限制在三个短句以内。

**Architecture:** 只修改 `SKILL.md`，不拆分辅助文件。把约束同步写入硬门、反模式、顺序清单、流程图、详细流程和输出规则，避免同一 skill 内出现互相冲突的教学指令。

**Tech Stack:** Markdown、YAML front matter、Graphviz DOT、Git、`rg`、Ruby 标准库 `yaml`

---

## 文件边界

- Modify: `SKILL.md` — 唯一运行时行为入口。
- Reference: `docs/superpowers/specs/2026-07-10-young-learner-guidance-design.md` — 已确认设计，只读。
- Do not modify: `references/hardware-list.md`、`references/project-list/quick_tap_duel/quick_tap_duel.md`。
- Do not modify: `/Users/a1234/Desktop/趣编程/.agents/skills/maker-launch-mentor/` — 本地测试副本本轮不覆盖。

### Task 1: 重建项目介绍与三题理解门

**Files:**
- Modify: `SKILL.md:13-25`
- Modify: `SKILL.md:42-69`
- Modify: `SKILL.md:71-137`

- [ ] **Step 1: 运行旧规则断言，确认当前行为不合格**

Run:

```bash
rg -n '目标、玩法、难度和需要准备的主要硬件|问一个小问题确认学员理解目标' SKILL.md
```

Expected: 命中旧规则；这是失败基线。

Run:

```bash
rg -n '游戏开始后要做什么|怎样算赢|哪种情况算犯规' SKILL.md
```

Expected: 无输出，退出码 1。

- [ ] **Step 2: 把 HARD-GATE 的理解条件改成三题全部通过**

将原“确认项目目标和基本要求”规则替换为：

```markdown
- 学员选择项目后，必须逐个确认“游戏开始后要做什么、怎样算赢、哪种情况算犯规”；三个问题没有全部答对之前，不要介绍硬件、进入制作或写代码。
```

将第一条资料规则收窄为 AI 内部准备：

```markdown
- 没有列出并读取 `references/project-list` 下的项目说明文件之前，不要推荐或开始项目。
```

- [ ] **Step 3: 更新 Anti-Patterns 与 Checklist**

加入反模式：

```markdown
- **“先讲硬件和难度才显得完整”**：错误。项目介绍只讲玩法；硬件、难度和专业术语会过早增加低龄学员的压力。
- **“问一个问题就算理解了”**：错误。必须逐个检查操作目标、获胜条件和犯规条件，三个问题全部答对后才能继续。
```

用以下内容替换 Checklist 的入口部分，后续模块步骤保持原顺序并重新连续编号：

```markdown
1. **读取项目资料** - 读取每个项目说明文件后才能介绍项目；读取是 AI 的内部准备。
2. **认识学员** - 逐个询问姓名、年龄、性别、年级，并做简短正向反馈。
3. **只介绍玩法** - 只说游戏怎么玩、孩子要做什么和有趣结果，不说难度、硬件或专业术语。
4. **确认选择** - 让学员选择一个项目。
5. **检查三条规则** - 依次询问游戏开始后要做什么、怎样算赢、哪种情况算犯规；每次只问一个。
6. **纠正并重问** - 答错时只用更简单的话重讲当前规则，然后重新提问。
7. **读取硬件清单** - 三题全部答对后，在判断硬件前读取 `references/hardware-list.md`。
8. **检查硬件** - 确认硬件是否齐全；缺硬件时暂停实物制作并提醒家长。
9. **摆放当前硬件** - 只拿出当前步骤会用到的硬件，需要时再观察。
```

- [ ] **Step 4: 改写详细入口流程**

将“认识学员”改成依次提问，不在一条回复中要求四项答案。将“介绍和选择项目”与新的规则检查写成：

```markdown
### 2. 介绍和选择项目

介绍项目之前必须已经读取项目说明文件。读取资料是 AI 的内部准备，不要把难度、硬件或专业术语提前讲给孩子。

介绍每个项目时，只说明游戏怎么玩、孩子要做什么、最后有什么有趣体验。介绍后让学员选择项目；不要在这个阶段说明项目难度、主要硬件、电路或编程概念。

### 3. 检查游戏规则

学员选择项目后，依次检查三个问题，每次只问一个：

1. 游戏开始后要做什么？
2. 怎样算赢？
3. 哪种情况算犯规？

问题要结合项目改写成孩子听得懂的说法，但必须分别检查操作目标、获胜条件和犯规条件。没有输赢或犯规的项目，分别改查成功条件和不能做的事情，仍保留三个检查点。

答对后再问下一题。答错或不完整时，不批评也不推进；只用更简单、更短的话重讲当前规则，然后重问当前题。三题全部答对后才能检查硬件。
```

将后续工作流小节重新连续编号。

- [ ] **Step 5: 改写 DOT 图入口**

用下列节点和连线替换原“确认理解目标”路径：

```dot
  "只介绍玩法" [shape=box];
  "学员选择项目" [shape=diamond];
  "逐个检查三条规则" [shape=box];
  "当前回答正确吗" [shape=diamond];
  "简化当前规则并重问" [shape=box];
  "三题全部答对吗" [shape=diamond];

  "了解学员信息" -> "只介绍玩法";
  "只介绍玩法" -> "学员选择项目";
  "学员选择项目" -> "逐个检查三条规则" [label="已选择"];
  "逐个检查三条规则" -> "当前回答正确吗";
  "当前回答正确吗" -> "简化当前规则并重问" [label="否"];
  "简化当前规则并重问" -> "逐个检查三条规则";
  "当前回答正确吗" -> "三题全部答对吗" [label="是"];
  "三题全部答对吗" -> "逐个检查三条规则" [label="否，问下一题"];
  "三题全部答对吗" -> "读取硬件清单" [label="是"];
```

- [ ] **Step 6: 验证并提交入口流程**

Run:

```bash
test "$(rg -c '游戏开始后要做什么' SKILL.md)" -ge 2
test "$(rg -c '怎样算赢' SKILL.md)" -ge 2
test "$(rg -c '哪种情况算犯规' SKILL.md)" -ge 2
! rg -n '目标、玩法、难度和需要准备的主要硬件|问一个小问题确认学员理解目标|项目大概难在哪里|会用到哪些主要硬件' SKILL.md
```

Expected: 全部退出码 0，最后一个 `rg` 无输出。

```bash
git add SKILL.md
git commit -m "feat: require gameplay understanding before making"
```

### Task 2: 增加危险工具家长安全门

**Files:**
- Modify: `SKILL.md:13-25`
- Modify: `SKILL.md:42-116`
- Modify: `SKILL.md:195-230`

- [ ] **Step 1: 运行危险工具失败断言**

Run:

```bash
rg -n '家长.*教过|全程陪同|不输出危险工具.*教程' SKILL.md
```

Expected: 无输出，退出码 1。

- [ ] **Step 2: 增加强制规则和反模式**

在 HARD-GATE 中加入：

```markdown
- 即将使用电烙铁、小型打磨机或其他可能造成人身伤害的危险工具时，必须先问家长是否教过；无论学员是否声称会用，家长没有到场并全程陪同前，不要继续该工具步骤。
- 不要输出危险工具的使用教程、动作分解或独立操作指导；具体操作必须由家长手把手指导。
```

在 Anti-Patterns 中加入：

```markdown
- **“孩子说会用工具就可以自己做”**：错误。危险工具无论是否学过，都必须由家长到场并全程陪同。
- **“给详细教程就足够安全”**：错误。AI 不讲危险工具怎么操作，具体使用必须由家长手把手指导。
```

- [ ] **Step 3: 把安全门加入 Checklist 和详细流程**

Checklist 在模块引导后增加“识别危险工具”和“等待家长陪同”，并重新编号。详细流程加入：

```markdown
### 危险工具安全门

危险工具包括当前资料里的电烙铁、小型打磨机，以及未来项目资料中标记为可能造成烫伤、割伤、夹伤、飞溅或其他人身伤害的工具。普通接线、插拔硬件和上传代码不自动视为危险工具操作；项目资料另有安全要求时，以项目资料为准。

即将使用危险工具时，一次只问一个问题：“家长教过你怎么使用它吗？”

- 没教过：让学员呼叫家长，暂停当前危险工具步骤。
- 教过或学员说自己会：仍要请家长到场并全程陪同。
- 家长未到场：不继续危险工具步骤。
- 家长已到场：由家长手把手指导具体操作。

AI 不输出危险工具使用教程、动作分解或独立操作指导，只说明项目需要完成的结果和完成后的观察、验证事项。
```

在模块强制规则中增加“遇到危险工具先执行安全门”和“AI 不讲工具具体用法”，在 Gotchas 中明确电烙铁与小型打磨机适用此门。

- [ ] **Step 4: 把安全门加入 DOT 图**

在“引导硬件和接线”之后加入：

```dot
  "当前步骤需要危险工具吗" [shape=diamond];
  "家长教过吗" [shape=diamond];
  "呼叫家长" [shape=box];
  "家长已到场并全程陪同吗" [shape=diamond];
  "暂停危险工具步骤" [shape=box];
  "由家长手把手指导" [shape=box];

  "引导硬件和接线" -> "当前步骤需要危险工具吗";
  "当前步骤需要危险工具吗" -> "进入编写代码步骤" [label="否"];
  "当前步骤需要危险工具吗" -> "家长教过吗" [label="是"];
  "家长教过吗" -> "呼叫家长" [label="否"];
  "家长教过吗" -> "家长已到场并全程陪同吗" [label="是"];
  "呼叫家长" -> "家长已到场并全程陪同吗";
  "家长已到场并全程陪同吗" -> "暂停危险工具步骤" [label="否"];
  "暂停危险工具步骤" -> "家长已到场并全程陪同吗";
  "家长已到场并全程陪同吗" -> "由家长手把手指导" [label="是"];
  "由家长手把手指导" -> "进入编写代码步骤";
```

- [ ] **Step 5: 验证并提交安全门**

Run:

```bash
test "$(rg -c '全程陪同' SKILL.md)" -ge 4
test "$(rg -c '不输出危险工具|不讲危险工具' SKILL.md)" -ge 3
rg -n '电烙铁.*小型打磨机|普通接线、插拔硬件和上传代码不自动视为危险工具操作' SKILL.md
```

Expected: 全部退出码 0，当前危险工具和普通操作边界均有命中。

```bash
git add SKILL.md
git commit -m "feat: require parent supervision for dangerous tools"
```

### Task 3: 按需讲硬件知识并限制孩子回复长度

**Files:**
- Modify: `SKILL.md:157-193`
- Modify: `SKILL.md:215-250`

- [ ] **Step 1: 运行长度规则失败断言**

Run:

```bash
rg -n '不超过三个短句|每次只推进一件事|拆成多轮' SKILL.md
```

Expected: 无输出，退出码 1。

- [ ] **Step 2: 将集中教学改为当前步骤按需教学**

将硬件摆放段落改为只拿当前步骤所需硬件。将电路知识标题和引导语改为：

```markdown
## 当前步骤需要时再讲的电路知识

不要在项目介绍阶段或制作前一次讲完所有电路术语。进入具体硬件步骤后，只在当前操作确实需要时，使用下面对应的机器狗类比；每次只讲一个概念，并用一个小问题检查理解。

机器狗类比的对应关系保持如下，不自行换说法：
```

保留七条现有类比，结尾改为：

```markdown
每次只选择当前步骤需要的一条类比。讲完后问一个简单问题检查理解；答对再继续，答错就把当前概念说得更简单，不要一次重复全部概念。
```

- [ ] **Step 3: 更新反模式、通用规则与输出风格**

加入“一次讲完更省时间”和“先讲完所有电路知识”的反模式。将输出风格完整替换为：

```markdown
## 输出风格

1. 面向孩子时，每次只推进一件事，每次回复通常不超过三个短句。
2. 面向孩子时，每次只问一个问题；认识学员、三题规则检查和概念检查都逐个提问。
3. 使用简单中文和短句，不用密集长句绕过长度限制。
4. 项目介绍只讲玩法，不讲难度、硬件或专业术语。
5. 必须讲术语时，等进入对应步骤再用一个简单类比解释。
6. 代码、接线检查和排错清单较长时拆成多轮，每轮只给当前小步骤。
7. 根据学员信息调整语气和例子。
8. 先问观察，再给一个提示，再让学员尝试。
9. 给代码前说明所属模块和要验证的现象。
10. 家长安全提醒保持简短、直接，但不能省略危险工具安全门。
```

在 Key Principles 加入“先玩法后硬件、理解后开始、一次一小步、危险工具家长陪同”。

- [ ] **Step 4: 验证并提交短回复规则**

Run:

```bash
test "$(rg -c '不超过三个短句' SKILL.md)" -ge 3
test "$(rg -c '拆成多轮' SKILL.md)" -ge 2
rg -n '不要在项目介绍阶段或制作前一次讲完所有电路术语|每次只选择当前步骤需要的一条类比' SKILL.md
! rg -n '^## 做项目前需要普及的电路知识$|如果硬件齐全，进入硬件摆放和基础知识讲解' SKILL.md
```

Expected: 全部退出码 0，最后一个 `rg` 无输出。

```bash
git add SKILL.md
git commit -m "feat: keep young learner guidance brief"
```

### Task 4: 完整一致性与范围验证

**Files:**
- Verify: `SKILL.md`
- Verify unchanged: `references/hardware-list.md`
- Verify unchanged: `references/project-list/quick_tap_duel/quick_tap_duel.md`

- [ ] **Step 1: 验证 YAML front matter**

Run:

```bash
ruby -e 'require "yaml"; text = File.read("SKILL.md"); data = YAML.safe_load(text.split(/^---\s*$\n/)[1]); abort "missing name" unless data["name"] == "maker-launch-mentor"; puts "front matter ok"'
```

Expected: `front matter ok`。

- [ ] **Step 2: 验证关键规则和旧规则清除**

Run:

```bash
test "$(rg -c '游戏开始后要做什么' SKILL.md)" -ge 2
test "$(rg -c '怎样算赢' SKILL.md)" -ge 2
test "$(rg -c '哪种情况算犯规' SKILL.md)" -ge 2
test "$(rg -c '全程陪同' SKILL.md)" -ge 4
test "$(rg -c '不超过三个短句' SKILL.md)" -ge 3
! rg -n '目标、玩法、难度和需要准备的主要硬件|问一个小问题确认学员理解目标|项目大概难在哪里|会用到哪些主要硬件|^## 做项目前需要普及的电路知识$' SKILL.md
```

Expected: 全部退出码 0，最后一个 `rg` 无输出。

- [ ] **Step 3: 验证 DOT 关键节点都有定义和连线**

Run:

```bash
for node in '逐个检查三条规则' '三题全部答对吗' '当前步骤需要危险工具吗' '家长已到场并全程陪同吗' '由家长手把手指导'; do count=$(rg -F -c "\"$node\"" SKILL.md); test "$count" -ge 2 || { echo "node not connected: $node"; exit 1; }; done; echo 'dot flow ok'
```

Expected: `dot flow ok`。

- [ ] **Step 4: 验证格式、范围和工作区**

Run:

```bash
git diff --check HEAD~3..HEAD
! rg -n 'T[O]DO|T[B]D|implement[[:space:]]+later|fill[[:space:]]+in[[:space:]]+details' SKILL.md
git diff --name-only HEAD~3..HEAD
```

Expected: 前两条无输出且退出码 0；最后只输出 `SKILL.md`。

Run from `/Users/a1234/Desktop/趣编程`:

```bash
git status --short .agents/skills/maker-launch-mentor
```

Expected: 无输出。

- [ ] **Step 5: 人工复核九个场景**

逐项阅读 `SKILL.md`，确认每项都有明确且无冲突的硬门和详细规则：

```text
1. 初次介绍只讲玩法。
2. 三题逐个问，答错只重讲并重问当前题。
3. 三题全对后才检查硬件。
4. 电烙铁步骤先确认家长是否教过，家长不到场就暂停。
5. 孩子说“我会”仍不能独自使用危险工具。
6. AI 不输出电烙铁或小型打磨机教程。
7. 普通接线、插拔和上传代码不被一律拦截。
8. 普通孩子回复不超过三个短句，一次只推进一件事。
9. 长代码、接线检查和排错内容拆成多轮。
```

Expected: 九项全部通过。若发现冲突，只修正 `SKILL.md`，然后重新执行 Task 4 的 Step 1-4。

- [ ] **Step 6: 确认独立仓库工作区干净**

Run:

```bash
git status --short
```

Expected: 无输出。本计划不发布、不打标签，也不覆盖本地测试副本。
