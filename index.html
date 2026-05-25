export default async function handler(req, res){
    try{
        console.log("1. Request received");
        
        const token = process.env.GITHUB_TOKEN;
        console.log("2. Token exists:", !!token);
        
        if(!token) {
            console.log("ERROR: Missing GITHUB_TOKEN");
            return res.status(500).json({ message: "Missing GitHub Token" });
        }

        const owner = 'hieu081';
        const repo = 'esp32-builder';
        const path = 'esp32-builder.ino';
        const code = req.body.code;
        
        console.log("3. Code length:", code?.length);

        const api = `https://api.github.com/repos/${owner}/${repo}/contents/${path}`;

        console.log("4. Fetching existing file...");
        const getFile = await fetch(api,{
            headers:{
                Authorization:`token ${token}`
            }
        });

        let sha = null;
        if(getFile.ok){
            const fileData = await getFile.json();
            sha = fileData.sha;
            console.log("5. Got SHA:", sha);
        } else {
            console.log("5. File not found, will create new");
        }

        const content = Buffer.from(code).toString('base64');
        console.log("6. Content encoded, length:", content.length);

        console.log("7. Uploading to GitHub...");
        const upload = await fetch(api,{
            method:'PUT',
            headers:{
                Authorization:`token ${token}`,
                'Content-Type':'application/json'
            },
            body:JSON.stringify({
                message:'Update ESP32 Code',
                content,
                sha
            })
        });

        console.log("8. GitHub response status:", upload.status);
        
        if(upload.ok){
            const result = await upload.json();
            console.log("9. Upload success:", result.content.path);
            return res.status(200).json({
                message:'Build Started - Code uploaded to GitHub'
            });
        } else {
            const errorText = await upload.text();
            console.log("9. Upload failed:", upload.status, errorText);
            return res.status(500).json({
                message:`GitHub Upload Failed: ${upload.status}`,
                details: errorText
            });
        }

    }catch(e){
        console.log("ERROR:", e);
        return res.status(500).json({
            message:e.toString()
        });
    }
}