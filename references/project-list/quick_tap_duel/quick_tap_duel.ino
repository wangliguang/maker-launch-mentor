
#include <Preferences.h>  
#include <TM1637Display.h>
#include <esp_system.h>  


const uint8_t DISPLAY_DIO_PIN = 1;  // 设置 TM1637 数码管 DIO 数据线连接到 GPIO1。
const uint8_t DISPLAY_CLK_PIN = 0;  // 设置 TM1637 数码管 CLK 时钟线连接到 GPIO0。

const uint8_t START_BUTTON_PIN = 4;  // 设置开始按钮连接到 GPIO4。

const uint8_t PLAYER1_LED_PIN = 5;  // 设置玩家1 LED 连接到 GPIO5。
const uint8_t PLAYER1_BUTTON_PIN = 6;  // 设置玩家1按钮连接到 GPIO6。

const uint8_t PLAYER2_LED_PIN = 7;  // 设置玩家2 LED 连接到 GPIO7。
const uint8_t PLAYER2_BUTTON_PIN = 8;  // 设置玩家2按钮连接到 GPIO8。

const unsigned long MAX_RANDOM_WAIT_MS = 5000;  // 设置随机等待的最长时间为 5000 毫秒。
const unsigned int NO_BEST_TIME = 0;  // 用 0 表示还没有保存过历史最快成绩。

const char* STORAGE_NAMESPACE = "quicktap";  // 设置 Preferences 存储空间的名字。
const char* BEST_TIME_KEY = "best_ms";  // 设置历史最快成绩在 Preferences 里的键名。

enum GameState {  // 定义游戏可能处于的几个状态。
  DEVELOPMENT_READY,  // 开发版初始准备阶段。
  WAITING_FOR_REFEREE,  // 开始初始化游戏，并裁判开始介入
  WAITING_FOR_PLAYER_TAP,  // 裁判已发令，等待玩家抢按。
  SHOWING_RESULT  // 显示结果。
};

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);  // 创建数码管对象，构造函数参数顺序是 CLK、DIO。
Preferences preferences;  // 创建 Preferences 对象，用来读取和保存历史最快成绩。

GameState gameState = DEVELOPMENT_READY;  // 当前游戏状态，开机时先设为开发版初始准备阶段。

unsigned int bestReactionMs = NO_BEST_TIME;  // 保存当前已知的历史最快反应时间。
unsigned long waitingStartedAt = 0;  // 保存本局随机等待开始时的 millis() 时间。
unsigned long randomWaitMs = 0;  // 保存本局随机等待要持续多少毫秒。
unsigned long activeStartedAt = 0;  // 保存进入可抢按状态时的 millis() 时间。

int lastStartButtonState = HIGH;  // 保存上一轮读取到的开始按钮状态，HIGH 表示未按下。

// 判断指定 GPIO 上的按钮是否被按下，按钮使用 INPUT_PULLUP，所以读到 LOW 表示按下
bool isButtonPressed(uint8_t pin) {  
  return digitalRead(pin) == LOW;  // 
}

// 判断开始按钮是否按下，按下响应
bool wasStartButtonJustPressed() {  
  int currentState = digitalRead(START_BUTTON_PIN);
  bool justPressed = lastStartButtonState == HIGH && currentState == LOW;
  lastStartButtonState = currentState;
  return justPressed;
}  

// 只管理两个玩家 LED。
void setPlayerLeds(bool player1Led, bool player2Led) {
  digitalWrite(PLAYER1_LED_PIN, player1Led ? HIGH : LOW);
  digitalWrite(PLAYER2_LED_PIN, player2Led ? HIGH : LOW);
}

// 在 TM1637 上显示数字。
void showNumber(unsigned long value) { 
  display.showNumberDec(value, false);
}

// 显示当前历史最快反应时间。
void showBestReaction() {  
  showNumber(bestReactionMs); 
}

// 显示四个横杠，表示随机等待中。
void showDashes() {
  const uint8_t DASH_SEGMENTS = SEG_G;
  uint8_t segments[] = {
    DASH_SEGMENTS,
    DASH_SEGMENTS,
    DASH_SEGMENTS,
    DASH_SEGMENTS
  };

  display.setSegments(segments);
}

// 显示 0，表示现在可以抢按。
void showGo() {
  showNumber(0);
}

//  显示某个玩家犯规，例如 F-1 或 F-2。
void showFoul(uint8_t playerNumber) {  
  const uint8_t LETTER_F_SEGMENTS = SEG_A | SEG_E | SEG_F | SEG_G;  // 手动组合字母 F 需要点亮的段。
  const uint8_t DASH_SEGMENTS = SEG_G;  // 横杠只需要点亮中间的 G 段。
  uint8_t segments[] = {  // 准备四位数码管每一位要显示的段码。
    LETTER_F_SEGMENTS,  // 第一位显示 F。
    DASH_SEGMENTS,  // 第二位显示横杠。
    display.encodeDigit(playerNumber),  // 第三位显示犯规玩家编号。
    0x00  // 第四位熄灭。
  };

  display.setSegments(segments);  // 把 F-1 或 F-2 的段码发送给数码管显示。
}

// 根据本局反应时间决定是否刷新历史最快。
void saveBestReaction(unsigned int reactionMs) {  // 
  if (bestReactionMs == NO_BEST_TIME || reactionMs < bestReactionMs) {
    bestReactionMs = reactionMs; 
    preferences.putUInt(BEST_TIME_KEY, bestReactionMs);

    Serial.print("最快反应时间: ");  
    Serial.print(bestReactionMs); 
    Serial.println(" ms");
  }  
}

// 刚启动，进入开发版初始准备阶段
void enterIdle() {  
  gameState = DEVELOPMENT_READY;  
  setPlayerLeds(false, false);
  showBestReaction();
}


// 初始化游戏
void initGame() {  
  
  waitingStartedAt = millis();
  randomWaitMs = random(MAX_RANDOM_WAIT_MS);

  setPlayerLeds(false, false);
  showDashes();
  gameState = WAITING_FOR_REFEREE;

  Serial.print("游戏开始，等待时间: ");
  Serial.print(randomWaitMs); 
  Serial.println(" ms");  
} 

// 裁判已发令，等待玩家抢按
void enterActive() {  
  gameState = WAITING_FOR_PLAYER_TAP;
  activeStartedAt = millis(); 
  setPlayerLeds(false, false);
  showGo();
}

// 显示犯规结果
void enterFoulResult(uint8_t playerNumber) {  
  gameState = SHOWING_RESULT; 
  setPlayerLeds(false, false);
  showFoul(playerNumber);

  Serial.print("犯规玩家：");  
  Serial.println(playerNumber); 
}

// 显示正常抢按获胜结果
void enterWinResult(uint8_t playerNumber, unsigned long rawReactionMs) {  
  gameState = SHOWING_RESULT;
  setPlayerLeds(playerNumber == 1, playerNumber == 2);
  showNumber(rawReactionMs);
  saveBestReaction(rawReactionMs);  // 如果本局更快，就保存为历史最快。

  Serial.print("赢的玩家：");
  Serial.print(playerNumber);
  Serial.print(", reaction: ");
  Serial.print(rawReactionMs);
  Serial.println(" ms"); 
}

/* 裁判发令期间要做的检查
 * 1. 玩家1是否在随机等待期间按下按钮
 * 2. 玩家2是否在随机等待期间按下按钮
 * 3. 等待时间是否结束
*/
void handleWaiting() {
  if (isButtonPressed(PLAYER1_BUTTON_PIN)) {
    enterFoulResult(1);
    return;
  }

  if (isButtonPressed(PLAYER2_BUTTON_PIN)) {
    enterFoulResult(2); 
    return; 
  }

  unsigned long now = millis();
  if (now - waitingStartedAt >= randomWaitMs) {
    enterActive(); 
  } 
}

// 裁判发令后检查谁获胜
void handleActive() {
  unsigned long now = millis();

  if (isButtonPressed(PLAYER1_BUTTON_PIN)) {
    enterWinResult(1, now - activeStartedAt);
    return; 
  }

  if (isButtonPressed(PLAYER2_BUTTON_PIN)) {
    enterWinResult(2, now - activeStartedAt);
    return;
  } 
}

void setup() {
  Serial.begin(115200);  // 启动串口，波特率设置为 115200。
  delay(500);  // 等待半秒，让串口和外设有时间准备好。

  pinMode(PLAYER1_LED_PIN, OUTPUT);  // 把玩家1 LED 引脚设置为输出模式。
  pinMode(PLAYER2_LED_PIN, OUTPUT);  // 把玩家2 LED 引脚设置为输出模式。

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);  // 把开始按钮引脚设置为内部上拉输入。
  pinMode(PLAYER1_BUTTON_PIN, INPUT_PULLUP);  // 把玩家1按钮引脚设置为内部上拉输入。
  pinMode(PLAYER2_BUTTON_PIN, INPUT_PULLUP);  // 把玩家2按钮引脚设置为内部上拉输入。

  setPlayerLeds(false, false);  // 开机后先确保两个玩家 LED 全部熄灭。

  display.setBrightness(7);  // 设置数码管亮度，7 是较亮的等级。
  display.clear();  // 清空数码管显示。

  preferences.begin(STORAGE_NAMESPACE, false);  // 打开 Preferences 存储空间，false 表示允许读写。
  bestReactionMs = preferences.getUInt(BEST_TIME_KEY, NO_BEST_TIME);  // 读取历史最快成绩，没有记录则得到 0。

  randomSeed(esp_random());  // 用 ESP32 硬件随机数初始化 Arduino 随机数种子。

  lastStartButtonState = digitalRead(START_BUTTON_PIN);  // 读取开机时开始按钮状态，避免刚开机误触发。
  enterIdle();  // 进入开发版初始准备阶段，显示历史最快成绩。

  Serial.println("双人反应力挑战机 初始化完毕");
  Serial.print("Best reaction: ");  
  Serial.print(bestReactionMs); 
  Serial.println(" ms"); 
}  

void loop() { 

  // 监听到开始按钮按下，开始初始化游戏，并裁判介入
  if (wasStartButtonJustPressed()) { 
    initGame();  
    return;
  } 

  // 裁判开始介入，监听发令枪
  if (gameState == WAITING_FOR_REFEREE) {
    handleWaiting();
    return;
  }

  // 发令枪已响，监听玩家
  if (gameState == WAITING_FOR_PLAYER_TAP) {
    handleActive();
    return;
  }
}
