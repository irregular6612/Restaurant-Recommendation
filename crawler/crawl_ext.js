const axios = require("axios");

const fs = require("fs");
const { type } = require("os");

const DB = "./dataset/DB_ext.txt";

let menuUrl = (menu) => {
  return `https://map.naver.com/p/api/search/allSearch?query=${menu}&type=all&searchCoord=126.842362%3B35.219526&boundary=`;
};

fs.writeFileSync(DB, "", (err) => console.log(err));
//fs.appendFileSync(DB, url2, (err) => console.log(err));
const crawlData = async (menu) => {
  try {
    const headers = {
      Accept: "application/json",
      "Accept-Encoding": "identity",
    };
    const headers1 = {};
    const response = await axios.get(menuUrl(menu), {
      headers: headers1,
    });
    //console.log(response.data);
    const storeList = response.data.result.place.list;
    //console.log(storeList[1]);
    //console.log(response.data.result.place.list[0]);

    for (storeInfo in storeList) {
      const storeIndex = response.data.result.place.list[storeInfo].index;
      const storeName = response.data.result.place.list[storeInfo].name;

      const storeTel = response.data.result.place.list[storeInfo].tel;
      const storeBizHourInfo =
        response.data.result.place.list[storeInfo].bizhourInfo;
      const storeMenuInfo = response.data.result.place.list[storeInfo].menuInfo;
      let storeCategory = response.data.result.place.list[storeInfo].category;
      storeCategory.forEach((element) => {
        //현재 가게별 카테고리 분류가 안되는 상황. 한식인데 CHINESE로 라벨링 됨. 수정 요망.
        ///*
        if (element.indexOf("한식")) {
          element += "KOREAN";
        } else if (element.indexOf("중식")) {
          element += "CHINESE";
        } else if (element.indexOf("일식")) {
          element += "JAPANESE";
        } else if (element.indexOf("카페") || element.indexOf("디저트")) {
          element += "CAFE";
        } else if (element.indexOf("이탈리안")) {
          element += "ITALIAN";
        } else if (element.indexOf("양식")) {
          element += "WESTERN";
        } else if (element.indexOf("패스트푸드")) {
          element += "FASTFOOD";
        } else if (element.indexOf("분식")) {
          element += "BOONSIK";
        } else {
          element += "ETC";
        }
        //*/
      });

      //fs.appendFileSync(DB, storeIndex + "번\t", (err) => console.log(err));
      fs.appendFileSync(DB, storeName + "\t", (err) => console.log(err));
      fs.appendFileSync(DB, storeCategory + "\n", (err) => console.log(err));
      fs.appendFileSync(DB, storeTel + "\t", (err) => console.log(err));
      fs.appendFileSync(DB, storeBizHourInfo + "\n", (err) => console.log(err));
      fs.appendFileSync(DB, storeMenuInfo + "\n\n", (err) => console.log(err));
    }

    return response;
  } catch (error) {
    console.error("Can't fetch the storeInfo", error.message);
    return null;
  }
};

crawlData("제육볶음");

///*
crawlData("햄버거");
crawlData("돌솥");
crawlData("돈까스");
crawlData("파스타");
crawlData("짜장면");
crawlData("국밥");
crawlData("찌개");
crawlData("보쌈");
crawlData("초밥");
crawlData("스시");
crawlData("해장국");
crawlData("닭갈비");
crawlData("갈비탕");
crawlData("라멘");
crawlData("냉면");
crawlData("순두부");
crawlData("낙지");
crawlData("카레");
crawlData("삼계탕");
crawlData("볶음밥");
crawlData("국수");
crawlData("삼겹살");
crawlData("스테이크");
crawlData("대게");
crawlData("곱창");
crawlData("육회");
crawlData("찜닭");
crawlData("장어");
crawlData("족발");
crawlData("찜");
crawlData("만두");
//*/
