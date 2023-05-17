#include "0key.h"
const char* key = APIKEY;
#include "playStory.h"
#include <WiFiClientSecure.h>
WiFiClientSecure client;
ChatGPT<WiFiClientSecure> chat_gpt(&client, "v1",  key);
char split_char[] = {',', '.', '!', '?', ';'};
void  split(String story, char *split_char) {
  int num_chars = sizeof(split_char) / sizeof(char);
  String phrase = "";
#ifdef CN
  story.replace("，", ",");
  story.replace("。", ".");
  story.replace("！", "!");
  story.replace("？", "?");
#endif
  for (int i = 0; i < story.length(); i++) {
    char c = story.charAt(i);
    bool is_split_char = false;

    for (int j = 0; j < num_chars; j++) {
      if (c == split_char[j]) {
        is_split_char = true;
        break;
      }
    }
    if (is_split_char) {
      if (phrase.length() > 0) {
        ttsThis(phrase);
        phrase = "";
      }
    } else {
      phrase += c;
    }
  }
  if (phrase.length() > 0) {
    ttsThis(phrase);
  }
}

void requestStory() {
  String result;
  client.setInsecure();
#ifdef CN
  if (chat_gpt.simple_message("gpt-3.5-turbo-0301", "user", "你是一个启蒙老师,请给我讲个短小的传统典故或者成语故事或者睡前故事或者关于星座神话传说的小故事.", result)) {
#else
  if (chat_gpt.simple_message("gpt-3.5-turbo-0301", "user", "you are a mentoring teacher. Please tell me a short classic bed time story, a story about Constellation,or an acient ancient myths or legends .", result)) {
#endif
    Serial.println("===OK===");
  } else {
    Serial.println("===ERROR===");
  }
  Serial.println(result);
  split(result, split_char);

}
