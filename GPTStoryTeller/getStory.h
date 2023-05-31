#include "timeAnn.h"
#include "0keySetup.h"
const char* key = APIKEY;
#include <WiFiClientSecure.h>
char split_char[] = {',', '.', '!', '?', ';'};
const char*  server = "api.openai.com";  // Server URL
const int httpsPort = 443;
const char* contentType = "application/json";
const char* url = "https://api.openai.com/v1/chat/completions";
const char* cnPrompt = "你是一位3-10岁儿童的辅导老师。请选取以下8个主题之一，用中文讲一个100-200字的经典睡前故事：1.古代神话或传说：从一个受神或女神干预影响的凡人的角度，讲述一个故事。这个凡人面临什么挑战，他们最终如何克服？2.历史小说：写一个真实的关于出生在19世纪初期的中国名人小时候的故事。从他的角度描述他面临的挑战以及他如何学会应对。3.童话故事：从被另一个角色对待不好的主角的角度，重新讲述“格林童话”系列中的一个故事。主角面临什么挑战，他们最终如何取得胜利？4.动物故事：写一个故事，讲述一个动物必须团结其他动物拯救他们的栖息地，以免被人类破坏。从主要动物角色的角度，描述他们面临的挑战以及他们最终如何成功。5.冒险故事：想象一个发现了一张神奇地图，带领他寻找宝藏并克服途中障碍的年轻男孩。从他的角度讲述他的冒险故事。6.带有寓意的睡前故事：写一个关于一个年轻女孩发现了一面魔镜，展示她的行为对他人的影响的故事。从她的角度描述她如何学会更加善良和有同情心地对待周围的人。7.幻想故事：写一个神话生物发现了通往人类世界的隐藏传送门的故事。从生物的角度描述他们面临的挑战，因为他们试图保护他们的世界免受威胁摧毁的邪恶势力。8.任意一个有意思的成语故事或歇后语故事，并且阐明其寓意。";
const char* enPrompt = "you are a mentoring teacher for kids from 3-10 years old. Please tell a short 100-200 words classic bed time story in english, use one of the following 7 topics:1. Ancient myths or legends: Retell a story from Greek mythology from the perspective of a mortal who is affected by the intervention of the gods or goddesses. What challenges does the mortal face, and how do they ultimately overcome them?2.Historical fiction: Write a story about a young girl living in the Australian outback during the early 1900s who must take on more responsibilities to help her family. From her perspective, describe the challenges she faces and how she learns to navigate them.3.Fairy tales: Retell a story from The Brothers Grimm collection from the perspective of the main character who is treated poorly by another character. What challenges does the main character face, and how do they ultimately triumph?4.Animal stories: Write a story about an animal who must rally other animals to save their habitat from destruction by humans. From the perspective of the main animal character, describe the challenges they face and how they ultimately succeed.5.Adventure stories: Imagine a young boy who discovers a magical map that leads to a hidden treasure. Write a story from his perspective as he sets off on an epic adventure to find the treasure and overcome obstacles along the way.6.Bedtime stories with a message: Write a story about a young girl who discovers a magic mirror that shows her the impact of her actions on others. From her perspective, describe how she learns to be kinder and more empathetic towards those around her.7.Fantasy stories: Write a story about a mythical creature who discovers a hidden portal that leads to a world of humans. From the creature's perspective, describe the challenges they face as they try to protect their world from an evil force that threatens to destroy it.";

void  split(String story, char *split_char) {
  int num_chars = sizeof(split_char) / sizeof(char);
  String phrase = "";
  if (CN) {
    story.replace("，", ",");
    story.replace("。", ".");
    story.replace("！", "!");
    story.replace("？", "?");
  }
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
        while (!play) {
          delay(100);
        }
      }
    } else {
      phrase += c;
    }
  }
  if (phrase.length() > 0) {
    ttsThis(phrase);
  }
}
bool chat_gpt(const char * request, String& result) {
  WiFiClientSecure client;
  client.setInsecure();
  if (!client.connect(server, httpsPort, 60000)) {
    Serial.println("Connection failed");
    char buf[256];
    int len = client.lastError(buf, sizeof(buf));
    Serial.print("Connection failed. Error message: ");
    Serial.println(String(buf).substring(0, len));
    return false;
  }
  String data = "{\"model\":\"gpt-3.5-turbo-0301\",\"messages\":[{\"role\":\"user\",\"content\":\"" + String(request) + "\"}],\"temperature\":0.5}";
  String header = "POST " + String(url) + " HTTP/1.1\r\n";
  header += "Host: " + String(server) + "\r\n";
  header += "Content-Type: " + String(contentType) + "\r\n";
  header += "Authorization: Bearer " + String(key) + "\r\n";
  header += "Content-Length: " + String(data.length()) + "\r\n\r\n";
  client.print(header);
  client.print(data);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  String response = client.readString();
  Serial.println(response);
  client.stop();
  // parse the JSON response and extract the answer
  int contentIndex = response.indexOf("\"content\":\"");
  // If the "content" field exists, extract its value
  if (contentIndex != -1) {
    contentIndex += 11; // Skip past the "content":" substring
    int endIndex = response.indexOf("\"},", contentIndex); // Find the end of the value
    result = response.substring(contentIndex, endIndex);
  } else {
    Serial.println("fail to decode json, The \"content\" field does not exist.");
    return false;
  }
  Serial.print("Question: " );
  Serial.println(request);
  Serial.println("Answer: " + result);
  return true;
}

void requestStory() {
 const  char* question= CN?cnPrompt:enPrompt;

  String result;
  finish = false;
  play = true;
  if (chat_gpt( question, result)) {
    Serial.println("===OK===");
  } else {
    Serial.println("===ERROR===");
  }
  Serial.println(result);
  obtain_time(CN);
  ttsThis(CN ? "亲爱的，让我来给你讲一个故事吧." : "Hi my dear, let me tell you a story.");
  split(result, split_char);
  delay(2000);
  ttsThis(CN ? "故事完." : "The End.");
  finish = true;
  play = false;
}
