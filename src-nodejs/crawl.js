const axios = require("axios");

const fs = require("fs");
const { type } = require("os");

const db = "./dataset/db.txt";

let menuUrl = (menu) => {
  return `https://map.naver.com/p/api/search/allSearch?query=${menu}&type=all&searchCoord=126.842362%3B35.219526&boundary=`;
};

fs.writeFileSync(db, "", (err) => console.log(err));
//fs.appendFileSync(db, url2, (err) => console.log(err));
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
    const storeVector = response.data.result.place.list;
    console.log(storeVector[1]);
    //console.log(response.data.result.place.list[0]);

    for (storeInfo in storeVector) {
      const hasMenu = response.data.result.place.list[storeInfo].menuExist;

      if (hasMenu === "1") {
        const storeIndex = response.data.result.place.list[storeInfo].index;
        const storeName = response.data.result.place.list[storeInfo].name;

        const storeTel = response.data.result.place.list[storeInfo].tel;
        const storeMenuInfo =
          response.data.result.place.list[storeInfo].menuInfo;
        let storeCategory = response.data.result.place.list[storeInfo].category;
        const reviewCount =
          response.data.result.place.list[storeInfo].reviewCount;

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

        //fs.appendFileSync(db, storeIndex + "번\t", (err) => console.log(err));
        fs.appendFileSync(db, storeName + "\t", (err) => console.log(err));
        fs.appendFileSync(db, storeCategory + "\n", (err) => console.log(err));
        fs.appendFileSync(db, storeTel + "\t", (err) => console.log(err));
        fs.appendFileSync(db, reviewCount + "\n", (err) => console.log(err));
        fs.appendFileSync(db, storeMenuInfo + "\n\n", (err) =>
          console.log(err)
        );
      }
    }

    return response;
  } catch (error) {
    console.error("Can't fetch the storeInfo", error.message);
    return null;
  }
};
crawlData("제육볶음");

///*
crawlData("돌솥");
crawlData("돈까스");
crawlData("파스타");
crawlData("짜장면");
//*/
