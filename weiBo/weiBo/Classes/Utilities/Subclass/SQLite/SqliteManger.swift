//
//  SqliteManger.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/12/7.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import FMDB

private let cacheTime = -10

class SqliteManger: NSObject {

    static let shared = SqliteManger()
    private var dbName: String = "T_Status"
    private var queue: FMDatabaseQueue?
    
    public func openDB(dbName: String) {
        //1. 创建并连接数据库
        self.dbName = dbName
        let documentsPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).last
        let path = (documentsPath! as NSString).appendingPathComponent(dbName)
        queue = FMDatabaseQueue(path: path)
        
        //2. 创建表
        createTable()
    }
    
    private func createTable() {
        let sql = "create table if not exists \"?\" (\"statusId\" integer not null,\"userId\" integer not null,\"status\" text,\"createTime\"  text not null,primary key(\"statusId\",\"userId\")) "
        queue?.inDatabase({ (db) in
            do {
                try db?.executeUpdate(sql, values: [self.dbName])
            }catch{
                print("error when create or read table")
            }
        })
    }
    
    /// 执行插入微博数据的操作
    ///
    /// - Parameter dictArray: 字典数组
    func updateStatus (dictArray: [[String: Any]]) {
        //db: FMDateBase的对象, 可以操作sql语句
        //roolBack: 是否回滚
        //datetime('now', 'localtime') 是sql中的一个函数, 它可以计算出一个日期 "yyyy-mm-dd HH:mm:ss"
        let sql = "insert or replace into ? (statusId,userId,status,createTime) values (?,?,?,datetime('now','localtime'));"
        for dict in dictArray {
            if let statusId = dict["statusId"] as? String,
                let userId = dict["userId"] as? String,
                let status = dict["status"] as? String {
                queue?.inTransaction({ (db,roolBack) in
                    do{
                        try db?.executeUpdate(sql, values: [self.dbName,statusId,userId,status])
                    }catch{
                        print("insert error")
                        roolBack?.pointee = true
                    }
                })
            }
        }
    }
    
    func <#name#>(<#parameters#>) -> <#return type#> {
        <#function body#>
    }
    
}
















