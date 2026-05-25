export default async function handler(req, res){
    try{

        const token = process.env.GITHUB_TOKEN;

        const owner = 'hieu081';

        const repo = 'esp32-builder';

        const path = 'esp32-builder.ino';

        const code = req.body.code;

        const api =
`https://api.github.com/repos/${owner}/${repo}/contents/${path}`;

        const getFile = await fetch(api,{

            headers:{
                Authorization:`token ${token}`
            }
        });

        let sha = null;

        if(getFile.ok){

            const fileData = await getFile.json();

            sha = fileData.sha;
        }

        const content =
        Buffer.from(code).toString('base64');

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

        if(upload.ok){

            return res.status(200).json({
                message:'Build Started'
            });
        }

        return res.status(500).json({
            message:'GitHub Upload Failed'
        });

    }catch(e){

        return res.status(500).json({
            message:e.toString()
        });
    }
}